//# 
//# Ready to compile programm
//# IF YOU DON'T KNOW WHAT YOU'RE DOING: DO NOT EDIT.
//# gcc --static [filename] to compile. 
//# FlatShell source for UNIX.
//#
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <pwd.h>
#include <errno.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
//# 
//# Built in commands
//# 

//# Clear screen
void flatsh_clrs(char **args) {
    printf("\033[2J\033[1;1H");
    fflush(stdout);
}
//# List directories
void flatsh_ldirs(char **args) {
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
}
//# Exit the shell.
void flatsh_escape(char **args) {
    exit(0);
}
//# Change the working directory.
void flatsh_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("fcd");
        }
    }
}
//# Flatfetch (shell and system info)
void flatsh_fetch(char **args) {
//# structs
    struct utsname buffer;
    struct sysinfo sysi;
//# cpu
    long cores = sysconf(_SC_NPROCESSORS_ONLN); 
    FILE *fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", "r");
    unsigned int max_freq_khz;
    fscanf(fp, "%u", &max_freq_khz);
//# ram
    sysinfo(&sysi);
    long long total_ram_mb = (long long)sysi.totalram * sysi.mem_unit / (1024 * 1024);
    uname(&buffer);
//# misc
    char *ver = "1.0" ;
    char *user = getenv("USER");
    char x = 'x';
    int num = strlen(user) + 2 ;
    char* str = (char*)malloc(num + 3);
    memset(str, x, num); 
    str[num] = '\0'; 
//# show info
    printf("....._=XYYYX=_..... X %s\n", user);
    printf("._X&$$$$$$$$$$$&X_. X%s \n", str);
    printf("&############--###& X Kernel - %s \n", buffer.sysname);
    printf("@#@#@#=++##++-#@#@# X Release - %s \n", buffer.release);
    printf("@@#@x==@#@@+++#@@#@ X Machine - %s \n", buffer.machine);
    printf("@@@@FFxxx====+@@@@@ X Nodename - %s \n", buffer.nodename);
    printf("X@@@@FFFFxx=@@@@@@X X Version - %s \n", buffer.version);
    printf("=xX&@@@@@@@@@@@&Xx= X FlatShell ver - %s \n", ver);
    printf("...-=xX&@@@&Xx=-... X Total freq * cores - %.2f MHZ * %ld CORES \n", max_freq_khz / 1000.0, cores);
    printf("                    X Total Ram Vol - %lld MB  \n\n", total_ram_mb);
    free(str);
}
//# Print current directory path
void flatsh_pwd(char **args) {
    char cwd[128];
    getcwd(cwd, sizeof(cwd));
    printf("%s\n", cwd);
}
//# Print flatshell help text.
void flatsh_help(char **args) {
    char *helptext =
        "FlatSh - Shell with unique ideas       .\n"
        "The following commands are builtins    :\n"
        "  fcd          Change working directory.\n"
        "  flatfetch    Print shell info        .\n"
        "  quit         Escape from the shell   .\n"
        "  help         Print this help text    .\n"
        "  fls          List subdirs            .\n"
        "  fpwd         Print current dir path  .\n"
        "  fclear       Clear screen            .\n"
        ;
    printf("%s", helptext);
}

//# end of Built in commands

//#
//# A builtin instance associates a command name with a handler function.
//#
struct builtin {
    char *name;
    void (*func)(char **args);
};
struct builtin builtins[] = {
    {"help", flatsh_help},
    {"quit", flatsh_escape},
    {"fcd", flatsh_cd},
    {"flatfetch", flatsh_fetch},
    {"fclear", flatsh_clrs},
    {"fpwd", flatsh_pwd},
    {"fls", flatsh_ldirs}
};
int flatsh_num_builtins() {
    return sizeof(builtins) / sizeof(struct builtin);
}

//# end of 

//# 
//# Process/command launcher.
//# 

void flatsh_exec(char **args) {
    for (int i = 0; i < flatsh_num_builtins(); i++) {
        if (strcmp(args[0], builtins[i].name) == 0) {
            builtins[i].func(args);
            return;
        }
    }

    pid_t child_pid = fork();

    if (child_pid == 0) {
        execvp(args[0], args);
        perror("flatsh");
        exit(1);
    } else if (child_pid > 0) {
        int status;
        do {
            waitpid(child_pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    } else {
        perror("flatsh");
    }
}

//# 
//# Input parser.
//# 
char** flatsh_split_line(char *line) {
    int length = 0;
    int capacity = 16;

    char **tokens = malloc(capacity * sizeof(char*));
    if (!tokens) {
        perror("flatsh");
        exit(1);
    }

    char *delimiters = " \t\r\n";
    char *token = strtok(line, delimiters);

    while (token != NULL) {
        tokens[length] = token;
        length++;

        if (length >= capacity) {
            capacity = (int) (capacity * 1.5);
            tokens = realloc(tokens, capacity * sizeof(char*));
            if (!tokens) {
                perror("flatsh");
                exit(1);
            }
        }

        token = strtok(NULL, delimiters);
    }

    tokens[length] = NULL;
    return tokens;
}
//# Read a single line of input from stdin. The return value is a string pointer
//# which needs to be freed once we're finished with it.
//#
char* flatsh_read_line() {
    char *line = NULL;
    size_t buflen = 0;
    errno = 0;
    ssize_t strlen = getline(&line, &buflen, stdin);
    if (strlen < 0) {
        if (errno) {
            perror("flatsh");
        }
        exit(1);
    }
    return line;
}

//#
//# Shell
//#

int main() {
	char *user = getenv("USER");
    while (true) {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        unsigned short width = w.ws_col;
        printf("%s # ", user);
        char *line = flatsh_read_line();
        char **tokens = flatsh_split_line(line);
        if (tokens[0] != NULL) {
            flatsh_exec(tokens);
        }
        free(tokens);
        free(line);
    }
}

