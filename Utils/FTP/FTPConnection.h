#ifndef FTPCONNECTION_H
#define FTPCONNECTION_H

#pragma once

#include <string>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>


class FTPConnection {
public:
    FTPConnection() : sock(-1) {}

    explicit FTPConnection(int socket) : sock(socket) {}

    ~FTPConnection() {
        close();
    }

    FTPConnection(FTPConnection&& other) noexcept : sock(other.sock) {
        other.sock = -1;
    }

    FTPConnection& operator=(FTPConnection&& other) noexcept {
        if (this != &other) {
            close();
            sock = other.sock;
            other.sock = -1;
        }
        return *this;
    }

    FTPConnection(const FTPConnection&) = delete;
    FTPConnection& operator=(const FTPConnection&) = delete;

    bool create() {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        return sock >= 0;
    }

    bool bind(uint16_t port) {
        if (sock < 0) return false;

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;

        int opt = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        return ::bind(sock, (sockaddr*)&addr, sizeof(addr)) == 0;
    }

    bool listen(int backlog = 1) {
        return sock >= 0 && ::listen(sock, backlog) == 0;
    }

    FTPConnection accept() {
        sockaddr_in clientAddr{};
        socklen_t len = sizeof(clientAddr);
        int clientSock = ::accept(sock, (sockaddr*)&clientAddr, &len);
        return FTPConnection(clientSock);
    }

    bool connect(const std::string& ip, uint16_t port) {
        if (sock < 0) return false;

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) != 1) return false;

        return ::connect(sock, (sockaddr*)&addr, sizeof(addr)) == 0;
    }

    bool sendAll(const std::string& data) const {
        const char* buf = data.c_str();
        size_t total = 0;
        size_t len = data.size();

        while (total < len) {
            ssize_t sent = ::send(sock, buf + total, len - total, 0);
            if (sent <= 0) return false;
            total += sent;
        }
        return true;
    }

    ssize_t recvSome(char* buffer, size_t length) const {
        return ::recv(sock, buffer, length, 0);
    }

    void close() {
        if (sock != -1) {
            ::close(sock);
            sock = -1;
        }
    }

    int getSocket() const {
        return sock;
    }

    bool isValid() const {
        return sock >= 0;
    }

private:
    int sock;
};


#endif //FTPCONNECTION_H
