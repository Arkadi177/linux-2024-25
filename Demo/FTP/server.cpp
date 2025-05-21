#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <filesystem>
#include <functional>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <system_error>
#include <vector>

namespace fs = std::filesystem;

constexpr auto OK_220 = "220 Service ready\r\n";
constexpr auto OK_200 = "200 Command OK\r\n";
constexpr auto USER_331 = "331 Username OK, need password\r\n";
constexpr auto PASS_230 = "230 Login successful\r\n";
constexpr auto NOTLOGGED_530 = "530 Not logged in\r\n";
constexpr auto GOODBYE_221 = "221 Goodbye\r\n";
constexpr auto START_150 = "150 Opening data connection\r\n";
constexpr auto COMPLETE_226 = "226 Transfer complete\r\n";
constexpr auto FILENF_550 = "550 File not found\r\n";
constexpr auto UNKNOWN_500 = "500 Unknown command\r\n";

using SocketCloser = std::function<void(int)>;
auto socketDeleter = [](int* sock) {
    if (sock && *sock >= 0) {
        close(*sock);
        delete sock;
    }
};
using SocketPtr = std::unique_ptr<int, decltype(socketDeleter)>;

// RAII wrapper for socket creation
SocketPtr makeSocket(int domain, int type, int protocol) {
    int s = socket(domain, type, protocol);
    if (s < 0) {
        throw std::system_error(errno, std::generic_category(), "socket creation failed");
    }
    return SocketPtr(new int(s), socketDeleter);
}

bool sendAll(int sock, const std::string& data) {
    size_t total = 0, len = data.size();
    while (total < len) {
        ssize_t sent = send(sock, data.data() + total, len - total, 0);
        if (sent <= 0) return false;
        total += sent;
    }
    return true;
}

std::optional<std::string> readLine(int sock) {
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

int makeListener(uint16_t port) {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("socket");
        return -1;
    }
    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in sa{};
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(port);

    if (bind(s, (sockaddr*)&sa, sizeof(sa)) < 0) {
        perror("bind");
        close(s);
        return -1;
    }
    if (listen(s, 1) < 0) {
        perror("listen");
        close(s);
        return -1;
    }
    return s;
}

std::optional<sockaddr_in> parsePortCommand(const std::string& arg) {
    // arg is like: h1,h2,h3,h4,p1,p2
    std::string s = arg;
    std::replace(s.begin(), s.end(), ',', ' ');
    std::istringstream iss(s);

    int h1, h2, h3, h4, p1, p2;
    if (!(iss >> h1 >> h2 >> h3 >> h4 >> p1 >> p2)) {
        return std::nullopt;
    }
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    std::string ip = std::to_string(h1) + "." + std::to_string(h2) + "." + std::to_string(h3) + "." + std::to_string(h4);
    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) != 1) return std::nullopt;
    addr.sin_port = htons(p1 * 256 + p2);
    return addr;
}

void handleClient(int ctrlSock) {
    sendAll(ctrlSock, OK_220);
    bool loggedIn = false;
    std::string username;
    std::optional<sockaddr_in> dataAddr;

    while (true) {
        auto lineOpt = readLine(ctrlSock);
        if (!lineOpt) break;
        std::string line = *lineOpt;

        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        // Convert cmd to uppercase for case-insensitive comparison
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

        if (cmd == "USER") {
            iss >> username;
            sendAll(ctrlSock, USER_331);
        } else if (cmd == "PASS") {
            std::string pass;
            iss >> pass;
            if (username == "admin" && pass == "secret123") {
                loggedIn = true;
                sendAll(ctrlSock, PASS_230);
            } else {
                sendAll(ctrlSock, NOTLOGGED_530);
            }
        } else if (cmd == "PORT") {
            std::string arg;
            std::getline(iss, arg);
            arg.erase(0, arg.find_first_not_of(" \t"));
            auto addrOpt = parsePortCommand(arg);
            if (addrOpt) {
                dataAddr = *addrOpt;
                sendAll(ctrlSock, OK_200);
            } else {
                sendAll(ctrlSock, UNKNOWN_500);
            }
        } else if (cmd == "QUIT") {
            sendAll(ctrlSock, GOODBYE_221);
            break;
        } else {
            if (!loggedIn) {
                sendAll(ctrlSock, NOTLOGGED_530);
                continue;
            }

            if (cmd == "PWD") {
                std::string cwd = fs::current_path().string();
                sendAll(ctrlSock, "257 \"" + cwd + "\"\r\n");
            } else if (cmd == "CWD") {
                std::string dir;
                iss >> dir;
                std::error_code ec;
                if (fs::exists(dir, ec) && fs::is_directory(dir, ec)) {
                    fs::current_path(dir, ec);
                    if (!ec) {
                        sendAll(ctrlSock, OK_200);
                    } else {
                        sendAll(ctrlSock, FILENF_550);
                    }
                } else {
                    sendAll(ctrlSock, FILENF_550);
                }
            } else if (cmd == "MKD") {
                std::string dir;
                iss >> dir;
                std::error_code ec;
                if (fs::create_directory(dir, ec) && !ec) {
                    sendAll(ctrlSock, OK_200);
                } else {
                    sendAll(ctrlSock, FILENF_550);
                }
            } else if (cmd == "RMD") {
                std::string dir;
                iss >> dir;
                std::error_code ec;
                if (fs::remove(dir, ec) && !ec) {
                    sendAll(ctrlSock, OK_200);
                } else {
                    sendAll(ctrlSock, FILENF_550);
                }
            } else if (cmd == "DELE") {
                std::string file;
                iss >> file;
                std::error_code ec;
                if (fs::remove(file, ec) && !ec) {
                    sendAll(ctrlSock, OK_200);
                } else {
                    sendAll(ctrlSock, FILENF_550);
                }
            } else if ((cmd == "LIST" || cmd == "RETR" || cmd == "STOR") && dataAddr) {
                sendAll(ctrlSock, START_150);

                SocketPtr dataSock = makeSocket(AF_INET, SOCK_STREAM, 0);
                if (connect(*dataSock, reinterpret_cast<const sockaddr*>(&*dataAddr), sizeof(*dataAddr)) < 0) {
                    sendAll(ctrlSock, FILENF_550);
                    continue;
                }

                if (cmd == "LIST") {
                    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
                        std::string name = entry.path().filename().string() + "\r\n";
                        sendAll(*dataSock, name);
                    }
                } else if (cmd == "RETR") {
                    std::string fname;
                    iss >> fname;
                    int filefd = open(fname.c_str(), O_RDONLY);
                    if (filefd < 0) {
                        sendAll(ctrlSock, FILENF_550);
                    } else {
                        char buf[4096];
                        ssize_t n;
                        while ((n = read(filefd, buf, sizeof(buf))) > 0) {
                            send(*dataSock, buf, n, 0);
                        }
                        close(filefd);
                    }
                } else if (cmd == "STOR") {
                    std::string fname;
                    iss >> fname;
                    int filefd = open(fname.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
                    if (filefd < 0) {
                        sendAll(ctrlSock, FILENF_550);
                    } else {
                        char buf[4096];
                        ssize_t n;
                        while ((n = recv(*dataSock, buf, sizeof(buf), 0)) > 0) {
                            write(filefd, buf, n);
                        }
                        close(filefd);
                    }
                }
                sendAll(ctrlSock, COMPLETE_226);
                dataAddr.reset();
            } else {
                sendAll(ctrlSock, UNKNOWN_500);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    uint16_t port = 2121;
    if (argc > 1) {
        port = static_cast<uint16_t>(std::stoi(argv[1]));
    }

    int listenfd = makeListener(port);
    if (listenfd < 0) {
        std::cerr << "Failed to listen on port " << port << "\n";
        return 1;
    }

    std::cout << "Server listening on port " << port << std::endl;

    while (true) {
        sockaddr_in cliAddr{};
        socklen_t cliLen = sizeof(cliAddr);
        int ctrlSock = accept(listenfd, reinterpret_cast<sockaddr*>(&cliAddr), &cliLen);
        if (ctrlSock < 0) {
            perror("accept");
            continue;
        }

        handleClient(ctrlSock);

        close(ctrlSock);
    }

    close(listenfd);
    return 0;
}
