//
// Created by khanh on 12/16/2022.
//
#include <iostream>
#include <unistd.h>

int main() {
    char buff[1024];
    int readResult;

    while (true) {
        readResult = read(0, buff, 1024);
        if (readResult == 0) return 0;
        buff[readResult] = 0;
        std::cout << "Printing: " << buff << std::endl;
    }
}