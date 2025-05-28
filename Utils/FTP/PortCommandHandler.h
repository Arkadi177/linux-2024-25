#ifndef PORTCOMMANDHANDLER_H
#define PORTCOMMANDHANDLER_H

#include "CommandHandler.h"
#include "../../Utils/FTP/FTPConnection.h"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

std::string readReply(FTPConnection& controlSock) {
    std::string reply;
    char c;
    while (true) {
        ssize_t ret = controlSock.recvSome(&c, 1);
        if (ret <= 0) break;
        reply.push_back(c);
        if (reply.size() >= 2 && reply.substr(reply.size()-2) == "\r\n") break;
    }
    std::cout << "<-- " << reply; // Print server reply for debug
    return reply;
}


class PortCommandHandler : public CommandHandler {
    std::string params;
    FTPConnection& dataListenerSocket;
    bool& portSet;
    FTPConnection& controlSocket; // needed to send PORT command to server

public:
    PortCommandHandler(const std::string& p, FTPConnection& dataSock, bool& portFlag, FTPConnection& ctrlSock)
        : params(p), dataListenerSocket(dataSock), portSet(portFlag), controlSocket(ctrlSock) {}

    void operator()() override {
        std::cout << "Executing PORT with params: " << params << "\n";

        std::vector<int> nums;
        std::stringstream ss(params);
        std::string token;

        while (std::getline(ss, token, ',')) {
            try {
                nums.push_back(std::stoi(token));
            } catch (...) {
                std::cerr << "Invalid PORT parameter: " << token << "\n";
                portSet = false;
                return;
            }
        }

        if (nums.size() != 6) {
            std::cerr << "PORT requires 6 comma-separated values\n";
            portSet = false;
            return;
        }

        int port = nums[4] * 256 + nums[5];

        if (!dataListenerSocket.create()) {
            std::cerr << "Failed to create data listener socket\n";
            portSet = false;
            return;
        }

        if (!dataListenerSocket.bind(port)) {
            std::cerr << "Failed to bind data listener socket\n";
            portSet = false;
            return;
        }

        if (!dataListenerSocket.listen()) {
            std::cerr << "Failed to listen on data listener socket\n";
            portSet = false;
            return;
        }

        // Send PORT command to server
        std::string portCmd = "PORT " + params + "\r\n";
        if (!controlSocket.sendAll(portCmd)) {
            std::cerr << "Failed to send PORT command to server\n";
            portSet = false;
            return;
        }
        std::string reply = readReply(controlSocket);
        if (reply.empty() || reply[0] != '2') { // expect 2xx reply for PORT success
            std::cerr << "Server rejected PORT command\n";
            portSet = false;
            return;
        }

        portSet = true;
        std::cout << "PORT command succeeded\n";
    }
};

#endif // PORTCOMMANDHANDLER_H
