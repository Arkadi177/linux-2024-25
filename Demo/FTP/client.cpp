// client.cpp
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <cerrno>
#include "../../Utils/Thread/ThreadPool.h"

int sendAll(int sock, const std::string& msg) {
    size_t total = 0;
    size_t len = msg.size();
    while (total < len) {
        ssize_t sent = send(sock, msg.data() + total, len - total, 0);
        if (sent <= 0) return 0;
        total += sent;
    }
    return 1;
}

int readLine(int sock, std::string& out) {
    out.clear();
    char c;
    while (recv(sock, &c, 1, 0) == 1) {
        if (c == '\r') continue;
        if (c == '\n') break;
        out.push_back(c);
    }
    return out.empty() ? -1 : 1;
}

void client_handler(int sock) {

}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port\n";
        return 1;
    }

    const char* host = argv[1];
    int port = std::atoi(argv[2]);

    int ctrlSock = socket(AF_INET, SOCK_STREAM, 0);
    if (ctrlSock < 0) {
        std::cerr << "Error creating control socket: " << strerror(errno) << "\n";
        return 1;
    }

    sockaddr_in servAddr{};
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, host, &servAddr.sin_addr) != 1) {
        std::cerr << "Invalid server IP address\n";
        close(ctrlSock);
        return 1;
    }

    if (connect(ctrlSock, (sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        std::cerr << "Failed to connect control socket: " << strerror(errno) << "\n";
        close(ctrlSock);
        return 1;
    }

    std::string line;
    ThreadPool threadPool;

    if (readLine(ctrlSock, line) > 0)
        std::cout << line << "\n";

    int dataListenerSock = -1;
    bool portSet = false;

    std::string inputLine;
    while (std::getline(std::cin, inputLine)) {
        if (inputLine.empty()) continue;

        std::istringstream iss(inputLine);
        std::string cmd;
        iss >> cmd;

        if (cmd == "PORT") {
            std::string params;
            std::getline(iss, params);
            params.erase(0, params.find_first_not_of(" \t"));

            std::vector<int> nums;
            std::istringstream pss(params);
            std::string token;
            while (std::getline(pss, token, ',')) {
                try {
                    nums.push_back(std::stoi(token));
                } catch (...) {
                    std::cerr << "Invalid PORT parameters.\n";
                    nums.clear();
                    break;
                }
            }
            if (nums.size() != 6) {
                std::cerr << "PORT command requires 6 comma-separated values.\n";
                continue;
            }

            std::string ipStr = std::to_string(nums[0]) + "." + std::to_string(nums[1]) + "." +
                                std::to_string(nums[2]) + "." + std::to_string(nums[3]);
            int dataPort = nums[4] * 256 + nums[5];

            if (dataListenerSock != -1) {
                close(dataListenerSock);
                dataListenerSock = -1;
            }

            dataListenerSock = socket(AF_INET, SOCK_STREAM, 0);
            if (dataListenerSock < 0) {
                std::cerr << "Failed to create data listener socket: " << strerror(errno) << "\n";
                continue;
            }

            sockaddr_in dataAddr{};
            dataAddr.sin_family = AF_INET;
            dataAddr.sin_port = htons(dataPort);
            dataAddr.sin_addr.s_addr = INADDR_ANY;

            int opt = 1;
            setsockopt(dataListenerSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

            if (bind(dataListenerSock, (sockaddr*)&dataAddr, sizeof(dataAddr)) < 0) {
                std::cerr << "Failed to bind data listener socket: " << strerror(errno) << "\n";
                close(dataListenerSock);
                dataListenerSock = -1;
                continue;
            }

            if (listen(dataListenerSock, 1) < 0) {
                std::cerr << "Failed to listen on data listener socket: " << strerror(errno) << "\n";
                close(dataListenerSock);
                dataListenerSock = -1;
                continue;
            }

            portSet = true;

            std::string portCmd = "PORT " + params + "\r\n";
            if (!sendAll(ctrlSock, portCmd)) {
                std::cerr << "Failed to send PORT command\n";
                break;
            }

            if (readLine(ctrlSock, line) > 0)
                std::cout << line << "\n";

        } else if (cmd == "LIST" || cmd == "RETR" || cmd == "STOR") {
            if (!portSet || dataListenerSock == -1) {
                std::cerr << "You must issue a PORT command before " << cmd << "\n";
                continue;
            }

            if (!sendAll(ctrlSock, inputLine + "\r\n")) {
                std::cerr << "Failed to send " << cmd << " command\n";
                break;
            }

            sockaddr_in serverDataAddr{};
            socklen_t serverDataLen = sizeof(serverDataAddr);
            int dataSock = accept(dataListenerSock, (sockaddr*)&serverDataAddr, &serverDataLen);
            if (dataSock < 0) {
                std::cerr << "Failed to accept data connection: " << strerror(errno) << "\n";
                continue;
            }

            std::string fullCmd = cmd;
            std::string filename;
            iss >> filename; // for RETR or STOR

            threadPool.enqueue([dataSock, ctrlSock, fullCmd, filename]()  {
                std::string line;

                if (fullCmd == "LIST") {
                    while (readLine(dataSock, line) > 0) {
                        std::cout << line << "\n";
                    }

                } else if (fullCmd == "RETR") {
                    if (filename.empty()) {
                        std::cerr << "RETR requires filename\n";
                    } else {
                        std::ofstream outFile(filename, std::ios::binary);
                        if (!outFile) {
                            std::cerr << "Cannot open file for writing: " << filename << "\n";
                        } else {
                            char buffer[4096];
                            ssize_t bytesRead;
                            while ((bytesRead = recv(dataSock, buffer, sizeof(buffer), 0)) > 0) {
                                outFile.write(buffer, bytesRead);
                            }
                        }
                    }

                } else if (fullCmd == "STOR") {
                    if (filename.empty()) {
                        std::cerr << "STOR requires filename\n";
                    } else {
                        std::ifstream inFile(filename, std::ios::binary);
                        if (!inFile) {
                            std::cerr << "Cannot open file for reading: " << filename << "\n";
                        } else {
                            char buffer[4096];
                            while (inFile.read(buffer, sizeof(buffer)) || inFile.gcount() > 0) {
                                ssize_t sent = send(dataSock, buffer, inFile.gcount(), 0);
                                if (sent <= 0) break;
                            }
                        }
                    }
                }

                close(dataSock);

                // Read response after data transfer
                if (readLine(ctrlSock, line) > 0) {
                    std::cout << line << "\n";
                }
            });

            close(dataListenerSock);
            dataListenerSock = -1;
            portSet = false;
        }
            else {
            std::string fullCmd = inputLine + "\r\n";
            if (!sendAll(ctrlSock, fullCmd)) {
                std::cerr << "Failed to send command\n";
                break;
            }

            if (readLine(ctrlSock, line) > 0) {
                std::cout << line << "\n";
            }
        }
    }

    if (dataListenerSock != -1) {
        close(dataListenerSock);
    }
    close(ctrlSock);

    return 0;
}
