#include <unistd.h>
#include <stdio.h>

int main() {
    char bufferToB[] = "This result is sent from A to B.\n";
    write(1, bufferToB, 1024);
    char bufferFromB[1024];

    FILE *f = fopen("resultA.txt", "w");
    fprintf(f, "Result from B: \n");

    while (true) {
        int ptr = read(0, bufferFromB, 1024);
        if (ptr == 0) {
            fclose(f);
            return 0;
        }
        bufferFromB[ptr] = 0;
        fprintf(f, "%s", bufferFromB);
        return 0;
    }
}