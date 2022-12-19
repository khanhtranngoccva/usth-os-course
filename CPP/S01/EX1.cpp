//
// Created by khanh on 12/16/2022.
//

#include <unistd.h>

int main() {
    int p[2];
    // Creates 2 file descriptors. P[1] will go to P[0]
    pipe(p);


    int pid = fork();

    // Child process
    if (pid == 0) {
        dup2(p[1], 1);
        close(p[1]);
        close(p[0]);
        execlp("./gendoc", "");
    } else {
        dup2(p[0], 0);
        close(p[1]);
        close(p[0]);
        execlp("./print", "");
    }
}
