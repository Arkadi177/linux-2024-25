#include <iostream>
#include <string>
#include "../../Utils/FTP/FTPConnection.h"
#include "../../Utils/FTP/CmdLineParser.h"

void CmdLineLoop(CmdLineParser& parser) {
    std::string inputLine;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, inputLine)) break;
        if (inputLine.empty()) continue;

        if (inputLine == "QUIT" || inputLine == "quit") {
            std::cout << "Exiting...\n";
            break;
        }

        auto handler = parser.parse(inputLine);
        if (handler) {
            (*handler)();
        }
    }
}

int main(int argc , char* argv[]) {
    FTPConnection controlSocket;
    if (!controlSocket.create()) {
        std::cerr << "Failed to create control socket\n";
        return 1;
    }

    if(argc < 2) {
        std::cerr << "Need two arguments\n";
        return 1;
    }

    std::string serverIP = argv[1];;

    if (!controlSocket.connect(serverIP, 2121)) {
        std::cerr << "Failed to connect to FTP server\n";
        return 1;
    }

    std::string welcome = readReply(controlSocket);

    FTPConnection dataListenerSocket;
    bool portSet = false;

    CmdLineParser parser(controlSocket, dataListenerSocket, portSet);

    CmdLineLoop(parser);

    controlSocket.close();
    return 0;
}
