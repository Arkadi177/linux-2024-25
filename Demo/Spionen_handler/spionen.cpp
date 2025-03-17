#include <algorithm>
#include <iostream>
#include <csignal>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <cstring>

void signalHandler(int sig, siginfo_t *info, void *context)
{
    if (sig == SIGCONT)
    {
        std::cout << "Spionen received SIGCONT from PID: " << info->si_pid << std::endl;

        if (kill(info->si_pid, 0) == -1)
        {
            std::cerr << "Invalid PID: " << info->si_pid << " (" << strerror(errno) << ")" << std::endl;
            return;
        }

        std::cout << "Responding with SIGILL to PID: " << info->si_pid << "\n";

        if (kill(info->si_pid, SIGILL) != 0)
        {
            std::cerr << "Failed to send SIGILL to PID: " << info->si_pid << " (" << strerror(errno) << ")" << std::endl;
        }
    }
}

int generateRandom4Digit()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1000, 9999);
    return dist(gen);
}

int main()
{
    std::ofstream file("postman.txt", std::ios::app);

    if (!file)
    {
        std::cerr << "Failed to open postman.txt" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<pid_t> pids = {getpid(), generateRandom4Digit(), generateRandom4Digit()};

    std::shuffle(pids.begin(), pids.end(), std::random_device());

    for (auto pid : pids)
    {
        std::cout << "Process or not? id : " << pid << std::endl;
        file << pid << " "; // Write PID to file
    }

    file.close();

    struct sigaction action;
    action.sa_sigaction = signalHandler;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);
    sigaction(SIGCONT, &action, nullptr);

    pause();

    return 0;
}
