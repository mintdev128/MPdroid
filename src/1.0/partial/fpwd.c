#include <stdio.h>
#include <unistd.h>
int main() {
    char cwd[128];
    getcwd(cwd, sizeof(cwd));
    printf("%s\n", cwd);
}