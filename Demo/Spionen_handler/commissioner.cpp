#include <iostream>
#include <csignal>
#include <fstream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <cstring>
#include <cerrno>
#include <unistd.h>

void signalHandler(int sig, siginfo_t *info, void *context)
{
    std::cout << "Received signal " << sig << std::endl;
    if (sig == SIGILL)
    {
        std::cout << "Spionen killed!!!!!!! " << info->si_pid << std::endl;
        kill(info->si_pid, SIGKILL);
    }
}

void findAndEliminateSpy()
{
    std::ifstream file("postman.txt");
    if (!file.is_open())
    {
        std::cerr << "Error opening postman.txt\n";
        return;
    }

    std::vector<pid_t> pids;
    std::string pid_string;

    while (file >> pid_string)
    {
        try
        {
            pids.push_back(std::stoi(pid_string));
        }
        catch (const std::exception &e)
        {
            std::cerr << "Invalid PID in file: " << pid_string << " (" << e.what() << ")\n";
        }
    }

    file.close();

    std::cout << "Commissioner scanning for spies...\n";

    sigset_t mask, old_mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGILL);
    sigprocmask(SIG_BLOCK, &mask, &old_mask);

    for (pid_t pid : pids)
    {
        if (kill(pid, SIGCONT) != 0)
        {
            std::cerr << "Failed to send SIGCONT to PID: " << pid << " (" << strerror(errno) << ")\n";
            continue;
        }

        struct sigaction sg{};
        sg.sa_sigaction = signalHandler;
        sg.sa_flags = SA_SIGINFO;
        sigemptyset(&sg.sa_mask);
        sigaction(SIGILL, &sg, nullptr);

        // Wait only for SIGILL, but continue immediately if another signal comes
        sigsuspend(&old_mask);
    }

    sigprocmask(SIG_SETMASK, &old_mask, nullptr);

    std::cout << "Commissioner operation complete.\n";
}

int main()
{
    findAndEliminateSpy();
    return 0;
}
