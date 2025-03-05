#include "do_command.h"
#include <iostream>
#include <unistd.h>

int main() {

    std::cout << "Clearing postman.txt" << std::endl;
    do_command("echo > postman.txt");

    std::cout << "Starting Spionen (Spy) processes..." << std::endl;
    do_command("./spionen_demo &");
    sleep(2);
    do_command("./spionen_demo &");
    sleep(2);
    do_command("./spionen_demo &");
    sleep(2);

    std::cout << "Starting Commissioner..." << std::endl;
    do_command("./comissioner_demo");
}
