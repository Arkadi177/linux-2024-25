#pragma once
#include "FTPConstants.h"
#include "FTPCommands.h"
#include "SocketUtils.h"
#include "AuthManager.h"
#include <algorithm>
#include <sstream>
#include <iostream>
#include <optional>

inline void handleClient(int ctrlSock) {
    sendAll(ctrlSock, OK_220);
    std::string username;
    std::string password;
    bool loggedIn = false;
    AuthManager auth;
    std::optional<sockaddr_in> dataAddr;

    while (true) {
        auto lineOpt = readLine(ctrlSock);
        if (!lineOpt) break;

        std::istringstream iss(*lineOpt);
        std::string cmd; iss >> cmd;
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

        if (cmd == "USER") {
            iss >> username;
            sendAll(ctrlSock, USER_331);
        } else if (cmd == "PASS") {
            iss >> password;
            if (auth.validateOrAdd(username, password)) {
                loggedIn = true;
                sendAll(ctrlSock, PASS_230);
            } else {
                sendAll(ctrlSock, NOTLOGGED_530);
            }
        } else if (cmd == "QUIT") {
            sendAll(ctrlSock, GOODBYE_221);
            break;
        } else if (cmd == "PORT") {
            std::string arg;
            std::getline(iss, arg);
            arg.erase(0, arg.find_first_not_of(" \t"));
            dataAddr = parsePortCommand(arg);
            sendAll(ctrlSock, dataAddr ? OK_200 : UNKNOWN_500);
        } else if (!loggedIn) {
            sendAll(ctrlSock, NOTLOGGED_530);
        } else if (cmd == "PWD" || cmd == "CWD" || cmd == "MKD" || cmd == "RMD" || cmd == "DELE") {
            handleSimpleCommands(ctrlSock, cmd, iss);
        } else if ((cmd == "LIST" || cmd == "RETR" || cmd == "STOR") && dataAddr) {
            std::string filename;
            if (cmd == "RETR" || cmd == "STOR") {
                iss >> filename;
                if (filename.empty()) {
                    sendAll(ctrlSock, FILENF_550);
                    continue;
                }
            }
            handleDataCommand(ctrlSock, cmd, filename, *dataAddr);
            dataAddr.reset();
        } else {
            sendAll(ctrlSock, UNKNOWN_500);
        }
    }
}

inline void startFTPServer(uint16_t port = 2121) {
    int listenfd = makeListener(port);
    std::cout << "Server listening on port " << port << std::endl;

    while (true) {
        sockaddr_in cliAddr{};
        socklen_t cliLen = sizeof(cliAddr);
        int ctrlSock = accept(listenfd, (sockaddr*)&cliAddr, &cliLen);
        if (ctrlSock >= 0) {
            handleClient(ctrlSock);
            close(ctrlSock);
        }
    }

    close(listenfd);
}
