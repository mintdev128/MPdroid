#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
//#Sometimes don't work
int main(int argc, char *argv[]) {
    if (argv[1] == NULL) {
        fprintf(stderr, "\n");
    } else {
        if (chdir(argv[1]) != 0) {
            perror("fcd");
        }
    }
    return 0;
}