#include <stdio.h>
#include <unistd.h>
int main() {
    printf("\033[2J\033[1;1H");
    fflush(stdout);
}