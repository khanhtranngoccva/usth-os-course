//
// Created by khanh on 12/16/2022.
//
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>

int main() {
    int pid1 = fork();
    int pid2 = fork();
    int pid3 = fork();

    waitpid(pid1, 0, 0);
    waitpid(pid2, 0, 0);
    waitpid(pid3, 0, 0);
    std::cout << getpid() << std::endl;
}