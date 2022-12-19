#include <cstdio>
#include <unistd.h>
int main()
{
    execlp( "cat", "resultA.txt");
    return 0;
}