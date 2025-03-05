#include <algorithm>
#include <iostream>
#include <csignal>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <random>
#include <string>

void signalHandler(int sig, siginfo_t *info, void *context)
{
    if (sig == SIGCONT)
    {
        std::cout << "Spionen received SIGCONT from PID: " << info->si_pid << std::endl;

        if (kill(info->si_pid, 0) == -1)
        {
            std::cerr << "Invalid PID: " << info->si_pid << " (" << std::strerror(errno) << ")" << std::endl;
            return;
        }

        std::cout << "Responding with SIGILL to PID: " << info->si_pid << "\n";

        if (kill(info->si_pid, SIGILL) != 0)
        {
            std::cerr << "Failed to send SIGILL to PID: " << info->si_pid << " (" << std::strerror(errno) << ")" << std::endl;
        }
    }
}

int generateRandom4Digit() {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1000, 9999);

    return dist(gen);
}

int main() {
    auto fd = open("postman.txt", O_RDWR | O_CREAT | O_APPEND);

    if(fd == -1)
    {
        std::cerr << "Failed to open file descriptor" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<pid_t> pids;

    pids.push_back(getpid());
    pids.push_back(generateRandom4Digit());
    pids.push_back(generateRandom4Digit());

    std::string str{};
    std::shuffle(pids.begin(), pids.end(), std::random_device());

    for(auto it : pids)
    {
        str = std::to_string(it) + " ";
        std::cout << "Process or not? id : " << it << std::endl;
        write(fd, str.c_str(), str.size());
    }

    close(fd);

    struct sigaction action;
    action.sa_sigaction = signalHandler;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);
    sigaction(SIGCONT, &action, nullptr);

    pause();

    return 0;
}

