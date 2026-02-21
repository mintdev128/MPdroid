#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
int main() {
    DIR *dir;
    struct dirent *entry;
    dir = opendir(".");
    if (dir == NULL) {
        perror("Unable to list subdirs");
    }
    while ((entry = readdir(dir)) != NULL) {
		printf("%s ", entry->d_name);
    }
    printf("\n");
    closedir(dir);
    return 0;
}