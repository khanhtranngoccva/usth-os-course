//
// Created by Admin on 12/16/2022.
//
#include <unistd.h>

int main() {
    execlp("./test", "test", "oof", "anotherTest");
}