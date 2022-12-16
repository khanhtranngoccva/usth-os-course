#include <unistd.h>

//
// Created by khanh on 12/16/2022.
//
int main() {
    int AtoB[2];
    int BtoA[2];

    pipe(AtoB);
    pipe(BtoA);

    int pid = fork();

    // Process A.
    if (pid == 0) {
        // Set stdout of A to AtoB.
        dup2(AtoB[1], 1);
        // Set stdin of A to BtoA.
        dup2(BtoA[0], 0);
        close(AtoB[1]);
        close(BtoA[0]);
        execlp("./A", "");
    }
    // Process B.
    else {
        // Set stdout of B to BtoA.
        dup2(BtoA[1], 1);
        // Set stdin of B to stdin of AtoB.
        dup2(AtoB[0], 0);
        close(AtoB[0]);
        close(BtoA[1]);
        execlp("./B", "");

    }
}