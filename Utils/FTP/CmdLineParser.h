#ifndef CMDLINEPARSER_H
#define CMDLINEPARSER_H

#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include "CommandHandler.h"
#include "PortCommandHandler.h"
#include "ListCommandHandler.h"
#include "RetrCommandHandler.h"
#include "StorCommandHandler.h"
#include "SimpleCommandHandler.h"

class CmdLineParser {
    FTPConnection& controlSocket;
    FTPConnection& dataListenerSocket;
    bool& portSet;

public:
    CmdLineParser(FTPConnection& ctrlSock, FTPConnection& dataSock, bool& portFlag)
    :   controlSocket(ctrlSock),
        dataListenerSocket(dataSock),
        portSet(portFlag)
        {}

    std::unique_ptr<CommandHandler> parse(const std::string& line)
    {
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        for (auto & c: cmd)
        {
            c = toupper(c);
        }

        if (cmd == "USER" || cmd == "PASS") {
            std::string arg;
            iss >> arg;
            return std::make_unique<SimpleCommandHandler>(cmd + " " + arg, controlSocket);
        }
        else if (cmd == "PWD") {
            return std::make_unique<SimpleCommandHandler>(cmd, controlSocket);
        }
        else if (cmd == "MKD" || cmd == "CWD") {
            std::string arg;
            iss >> arg;
            return std::make_unique<SimpleCommandHandler>(cmd + " " + arg, controlSocket);
        }
        else if (cmd == "PORT")
        {
            std::string params;
            std::getline(iss, params);
            if (!params.empty() && params[0] == ' ') params.erase(0,1);
            return std::make_unique<PortCommandHandler>(params, dataListenerSocket, portSet, controlSocket);
        }
        else if (cmd == "LIST")
        {
            return std::make_unique<ListCommandHandler>(controlSocket, dataListenerSocket, portSet);
        }
        else if (cmd == "RETR")
        {
            std::string filename;
            iss >> filename;
            if (filename.empty())
            {
                std::cerr << "RETR requires a filename\n";
                return nullptr;
            }
            return std::make_unique<RetrCommandHandler>(filename, controlSocket, dataListenerSocket, portSet);
        }
        else if (cmd == "STOR")
        {
            std::string filename;
            iss >> filename;
            if (filename.empty())
            {
                std::cerr << "STOR requires a filename\n";
                return nullptr;
            }
            return std::make_unique<StorCommandHandler>(filename, controlSocket, dataListenerSocket, portSet);
        }
        else
        {
            std::cerr << "Unknown command: " << cmd << "\n";
            return nullptr;
        }
    }
};

#endif // CMDLINEPARSER_H
