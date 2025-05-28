#ifndef RETRCOMMANDHANDLER_H
#define RETRCOMMANDHANDLER_H

#include "CommandHandler.h"
#include "../../Utils/FTP/FTPConnection.h"
#include <string>
#include <iostream>
#include <fstream>

class RetrCommandHandler : public CommandHandler {
    std::string filename;
    FTPConnection& controlSocket;
    FTPConnection& dataListenerSocket;
    bool& portSet;

public:
    RetrCommandHandler(const std::string& file, FTPConnection& ctrlSock, FTPConnection& dataSock, bool& portFlag)
        : filename(file), controlSocket(ctrlSock), dataListenerSocket(dataSock), portSet(portFlag) {}

    void operator()() override {
        if (!portSet) {
            std::cerr << "PORT command not set. Use PORT before RETR.\n";
            return;
        }

        std::string cmd = "RETR " + filename + "\r\n";
        if (!controlSocket.sendAll(cmd)) {
            std::cerr << "Failed to send RETR command\n";
            return;
        }

        std::string reply = readReply(controlSocket);
        if (reply.empty() || reply[0] != '1') { // expect 1xx opening data connection
            std::cerr << "Server rejected RETR command\n";
            return;
        }

        FTPConnection dataConn = dataListenerSocket.accept();
        if (!dataConn.isValid()) {
            std::cerr << "Failed to accept data connection\n";
            return;
        }

        std::ofstream outFile(filename, std::ios::binary);
        if (!outFile) {
            std::cerr << "Cannot open file for writing: " << filename << "\n";
            dataConn.close();
            return;
        }

        char buffer[4096];
        ssize_t bytesRead;
        while ((bytesRead = dataConn.recvSome(buffer, sizeof(buffer))) > 0) {
            outFile.write(buffer, bytesRead);
        }

        outFile.close();
        dataConn.close();

        reply = readReply(controlSocket);
    }
};

#endif // RETRCOMMANDHANDLER_H
