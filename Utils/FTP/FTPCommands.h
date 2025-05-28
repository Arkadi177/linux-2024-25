#pragma once
#include "SocketUtils.h"
#include "FTPConstants.h"
#include <filesystem>
#include <fcntl.h>
#include <unistd.h>

namespace fs = std::filesystem;

inline void handleSimpleCommands(int ctrlSock, const std::string& cmd, std::istringstream& iss) {
    if (cmd == "PWD") {
        sendAll(ctrlSock, "257 \"" + fs::current_path().string() + "\"\r\n");
    } else if (cmd == "CWD") {
        std::string dir; iss >> dir;
        std::error_code ec;
        if (fs::exists(dir, ec) && fs::is_directory(dir, ec)) {
            fs::current_path(dir, ec);
            sendAll(ctrlSock, ec ? FILENF_550 : OK_200);
        } else {
            sendAll(ctrlSock, FILENF_550);
        }
    } else if (cmd == "MKD") {
        std::string dir; iss >> dir;
        std::error_code ec;
        sendAll(ctrlSock, fs::create_directory(dir, ec) && !ec ? OK_200 : FILENF_550);
    } else if (cmd == "RMD" || cmd == "DELE") {
        std::string target; iss >> target;
        std::error_code ec;
        sendAll(ctrlSock, fs::remove(target, ec) && !ec ? OK_200 : FILENF_550);
    }
}

inline void handleDataCommand(int ctrlSock, const std::string& cmd, const std::string& filename, sockaddr_in dataAddr) {
    sendAll(ctrlSock, START_150);
    auto dataSock = makeSocket(AF_INET, SOCK_STREAM, 0);
    if (connect(*dataSock, (sockaddr*)&dataAddr, sizeof(dataAddr)) < 0) {
        sendAll(ctrlSock, FILENF_550);
        return;
    }

    if (cmd == "LIST") {
        for (auto& entry : fs::directory_iterator(fs::current_path())) {
            sendAll(*dataSock, entry.path().filename().string() + "\r\n");
        }
    } else if (cmd == "RETR") {
        int fd = open(filename.c_str(), O_RDONLY);
        if (fd < 0) {
            sendAll(ctrlSock, FILENF_550);
        } else {
            char buf[4096];
            ssize_t n;
            while ((n = read(fd, buf, sizeof(buf))) > 0) {
                send(*dataSock, buf, n, 0);
            }
            close(fd);
        }
    } else if (cmd == "STOR") {
        int fd = open(filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (fd < 0) {
            sendAll(ctrlSock, FILENF_550);
        } else {
            char buf[4096];
            ssize_t n;
            while ((n = recv(*dataSock, buf, sizeof(buf), 0)) > 0) {
                write(fd, buf, n);
            }
            close(fd);
        }
    }

    sendAll(ctrlSock, COMPLETE_226);
}
