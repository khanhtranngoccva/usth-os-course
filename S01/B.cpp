//
// Created by khanh on 12/16/2022.
//
#include <unistd.h>
#include <stdio.h>

int main() {
    char bufferToA[] = "This result is sent from B to A.\n";
    write(1, bufferToA, 1024);
    char bufferFromA[1024];

    FILE *f = fopen("resultB.txt", "w");
    fprintf(f, "Result from A: \n");
    while (true) {
        int ptr = read(0, bufferFromA, 1024);
        if (ptr == 0) {
            fclose(f);
            return 0;
        }
        bufferFromA[ptr] = 0;
        fprintf(f, "%s", bufferFromA);
        return 0;
    }
}