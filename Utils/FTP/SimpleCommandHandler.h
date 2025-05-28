// SimpleCommandHandler.h

#ifndef SIMPLECOMMANDHANDLER_H
#define SIMPLECOMMANDHANDLER_H

#include "CommandHandler.h"
#include "../../Utils/FTP/FTPConnection.h"
#include <string>
#include <iostream>

class SimpleCommandHandler : public CommandHandler {
    std::string commandLine;
    FTPConnection& controlSocket;

    // Helper to send command and read reply
    void sendAndPrintReply() {
        if (!controlSocket.sendAll(commandLine + "\r\n")) {
            std::cerr << "Failed to send command: " << commandLine << "\n";
            return;
        }

        std::string reply;
        char c;
        while (true) {
            ssize_t ret = recv(controlSocket.getSocket(), &c, 1, 0);
            if (ret <= 0) break;
            reply += c;
            if (reply.size() >= 2 && reply.substr(reply.size()-2) == "\r\n") {
                break;
            }
        }
        std::cout << "Server: " << reply;
    }

public:
    SimpleCommandHandler(const std::string& cmd, FTPConnection& ctrlSock)
        : commandLine(cmd), controlSocket(ctrlSock) {}

    void operator()() override {
        sendAndPrintReply();
    }
};

#endif // SIMPLECOMMANDHANDLER_H
