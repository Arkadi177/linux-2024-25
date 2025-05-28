#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <functional>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <system_error>

inline bool sendAll(int sock, const std::string& data) {
    size_t total = 0;
    while (total < data.size()) {
        ssize_t sent = send(sock, data.data() + total, data.size() - total, 0);
        if (sent <= 0) return false;
        total += sent;
    }
    return true;
}

inline std::optional<std::string> readLine(int sock) {
    std::string result;
    char c;
    while (true) {
        ssize_t r = recv(sock, &c, 1, 0);
        if (r <= 0) return std::nullopt;
        if (c == '\r') continue;
        if (c == '\n') break;
        result.push_back(c);
    }
    return result.empty() ? std::nullopt : std::optional<std::string>{result};
}

inline int makeListener(uint16_t port) {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) throw std::runtime_error("Socket creation failed");

    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in sa{};
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(port);

    if (bind(s, (sockaddr*)&sa, sizeof(sa)) < 0)
        throw std::runtime_error("Bind failed");

    if (listen(s, 1) < 0)
        throw std::runtime_error("Listen failed");

    return s;
}

inline std::optional<sockaddr_in> parsePortCommand(const std::string& arg) {
    std::string s = arg;
    std::replace(s.begin(), s.end(), ',', ' ');
    std::istringstream iss(s);

    int h1, h2, h3, h4, p1, p2;
    if (!(iss >> h1 >> h2 >> h3 >> h4 >> p1 >> p2)) return std::nullopt;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    std::string ip = std::to_string(h1) + "." + std::to_string(h2) + "." +
                     std::to_string(h3) + "." + std::to_string(h4);
    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) != 1) return std::nullopt;

    addr.sin_port = htons(p1 * 256 + p2);
    return addr;
}

struct SocketDeleter {
    void operator()(int* sock) const {
        if (sock && *sock >= 0) {
            close(*sock);
            delete sock;
        }
    }
};

using SocketPtr = std::unique_ptr<int, SocketDeleter>;

inline SocketPtr makeSocket(int domain, int type, int protocol) {
    int s = socket(domain, type, protocol);
    if (s < 0) throw std::system_error(errno, std::generic_category(), "Socket creation failed");
    return SocketPtr(new int(s));
}
