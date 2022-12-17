//
// Created by khanh on 12/16/2022.
//
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>

int main() {
    int psToGrep[2];
    int grepToWc[2];
    pipe(psToGrep);
    pipe(grepToWc);

    int pid1 = fork();
    int pid2 = fork();

    // Process for ps.
    if (pid1 && pid2) {
        dup2(psToGrep[1], 1);
        close(psToGrep[1]);
        close(psToGrep[0]);
        close(grepToWc[1]);
        close(grepToWc[0]);
        execlp("ps", "ps", "-ef", NULL);
        return 0;
    } else if (pid1) {
        dup2(psToGrep[0], 0);
        dup2(grepToWc[1], 1);
        close(psToGrep[1]);
        close(psToGrep[0]);
        close(grepToWc[1]);
        close(grepToWc[0]);
        execlp("grep", "grep", "firefox", NULL);
        return 0;
    } else if (pid2) {
        dup2(grepToWc[0], 0);
        close(psToGrep[1]);
        close(psToGrep[0]);
        close(grepToWc[1]);
        close(grepToWc[0]);
        execlp("wc", "wc", "-l", NULL);
        return 0;
    }
}

