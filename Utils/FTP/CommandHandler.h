#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H
#include <string>

class CommandHandler {
public:
    virtual void operator()() = 0;
    virtual ~CommandHandler() = default;
};
#endif //COMMANDHANDLER_H
