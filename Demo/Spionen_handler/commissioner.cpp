#include <iostream>
#include <csignal>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <cstring>

void signalHandler(int sig, siginfo_t *info, void *context)
{
    std::cout << "Received signal " << sig << std::endl;
    if (sig == SIGILL)
    {
        std::cout << "Spionen kiled!!!!!!! "  << info->si_pid << std::endl;
        kill(info->si_pid, SIGKILL);
    }
}

void findAndEliminateSpy()
{
    auto fd = open("postman.txt", O_RDONLY);
    if (fd == -1)
    {
        std::cerr << "Error opening postman.txt\n";
        return;
    }

    std::vector<pid_t> pids;
    std::string pid_string;

    char buffer;
    while (read(fd, &buffer, 1) > 0)
    {
        if (buffer != ' ' && buffer != '\n')
        {
            pid_string.push_back(buffer);
        }
        else if (!pid_string.empty())
        {
            pids.push_back(std::stoi(pid_string));
            pid_string.clear();
        }
    }

    close(fd);

    std::cout << "Commissioner scanning for spies...\n";

    for (pid_t pid : pids)
    {
        if(kill(pid, SIGCONT) != 0)
        {
            continue;
        }

        struct sigaction sg;
        sg.sa_sigaction = signalHandler;
        sg.sa_flags = SA_SIGINFO;
        sigemptyset(&sg.sa_mask);
        sigaction(SIGILL, &sg, nullptr);

        pause();
    }

    std::cout << "Commissioner operation complete.\n";
}

int main()
{
    findAndEliminateSpy();
    return 0;
}
