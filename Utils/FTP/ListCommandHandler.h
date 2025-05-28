#ifndef LISTCOMMANDHANDLER_H
#define LISTCOMMANDHANDLER_H

#include "CommandHandler.h"
#include "../../Utils/FTP/FTPConnection.h"
#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class ListCommandHandler : public CommandHandler {
    FTPConnection& controlSocket;
    FTPConnection& dataListenerSocket;
    bool& portSet;

public:
    ListCommandHandler(FTPConnection& ctrlSock, FTPConnection& dataSock, bool& portFlag)
        : controlSocket(ctrlSock), dataListenerSocket(dataSock), portSet(portFlag) {}

    void operator()() override {
        if (!portSet) {
            std::cerr << "PORT command not set. Use PORT before LIST.\n";
            return;
        }

        std::string cmd = "LIST\r\n";
        if (!controlSocket.sendAll(cmd)) {
            std::cerr << "Failed to send LIST command\n";
            return;
        }
        std::string reply = readReply(controlSocket);
        if (reply.empty() || reply[0] != '1') { // expect 1xx opening data connection
            std::cerr << "Server rejected LIST command\n";
            return;
        }

        FTPConnection dataConn = dataListenerSocket.accept();
        if (!dataConn.isValid()) {
            std::cerr << "Failed to accept data connection\n";
            return;
        }

        // Receive data from server and output to console
        char buffer[4096];
        ssize_t bytesRead;
        while ((bytesRead = dataConn.recvSome(buffer, sizeof(buffer))) > 0) {
            std::cout.write(buffer, bytesRead);
        }

        dataConn.close();

        reply = readReply(controlSocket); // read final reply after transfer
    }
};

#endif // LISTCOMMANDHANDLER_H
