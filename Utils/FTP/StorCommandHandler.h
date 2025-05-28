#ifndef STORCOMMANDHANDLER_H
#define STORCOMMANDHANDLER_H

#include "CommandHandler.h"
#include "../../Utils/FTP/FTPConnection.h"
#include <string>
#include <iostream>
#include <fstream>


class StorCommandHandler : public CommandHandler {
    std::string filename;
    FTPConnection& controlSocket;
    FTPConnection& dataListenerSocket;
    bool& portSet;

public:
    StorCommandHandler(const std::string& file, FTPConnection& ctrlSock, FTPConnection& dataSock, bool& portFlag)
        : filename(file), controlSocket(ctrlSock), dataListenerSocket(dataSock), portSet(portFlag) {}

    void operator()() override {
        if (!portSet) {
            std::cerr << "PORT command not set. Use PORT before STOR.\n";
            return;
        }

        std::string cmd = "STOR " + filename + "\r\n";
        if (!controlSocket.sendAll(cmd)) {
            std::cerr << "Failed to send STOR command\n";
            return;
        }

        std::string reply = readReply(controlSocket);
        if (reply.empty() || reply[0] != '1') { // expect 1xx opening data connection
            std::cerr << "Server rejected STOR command\n";
            return;
        }

        FTPConnection dataConn = dataListenerSocket.accept();
        if (!dataConn.isValid()) {
            std::cerr << "Failed to accept data connection\n";
            return;
        }

        std::ifstream inFile(filename, std::ios::binary);
        if (!inFile) {
            std::cerr << "Cannot open file for reading: " << filename << "\n";
            dataConn.close();
            return;
        }

        char buffer[4096];
        while (!inFile.eof()) {
            inFile.read(buffer, sizeof(buffer));
            std::streamsize bytesRead = inFile.gcount();
            if (!dataConn.sendAll(std::string(buffer, bytesRead))) {
                std::cerr << "Failed to send data\n";
                break;
            }
        }

        inFile.close();
        dataConn.close();

        reply = readReply(controlSocket);
    }
};

#endif // STORCOMMANDHANDLER_H
