//# 
//# Ready to compile programm
//# IF YOU DON'T KNOW WHAT YOU'RE DOING: DO NOT EDIT.
//# gcc --static --lncurses [filename] to compile. 
//# FlatShell source for LINUX.
//#
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <ncurses.h>
#include <pwd.h>
#include <errno.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <readline/readline.h>
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif
//#define COLOR "\033[(48 or 38);2;R;G;B"
#define BK "\x1B[38;02;02;02;02;48;2;22;22;22m"
#define RD "\x1B[31m"
#define GN "\x1B[32m"
#define YL "\x1B[33m"
#define BL "\x1B[34m"
#define MG "\x1B[35m"
#define CY "\x1B[36m"
#define WH "\x1B[40;37m"
#define NO "\x1B[0m" 
#define DGR "\033[48;2;22;22;22;38;2;36;36;36m"
#define GR "\033[48;2;22;22;22;38;2;63;63;63m"
#define FLAME "\033[48;2;133;0;0;38;2;43;0;0m"
#define MENU "\033[48;2;130;0;0;38;2;222;222;222m"
#define ERRcol "\033[48;2;190;0;0;38;2;0;0;0m"
#define CLEAR "\033[2J\033[1;1H"
//#
//# Functions 
//#
void centr(char *text, int num) {
    int cols = 80;
    int lines = 24;
#ifdef TIOCGSIZE
    struct ttysize ts;
    ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
    cols = ts.ts_cols;
    lines = ts.ts_lines;
#elif defined(TIOCGWINSZ)
    struct winsize ts;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
    cols = ts.ws_col;
    lines = ts.ws_row;
#endif
    int centrl = (cols/2) - (strlen(text)/2);
    for (int i = 0; i < centrl; i++) {
        if (num = 0){printf(" ");printf("%s",text);}
        else if (num = 1){mvprintw(3,centrl,"%s",text);}
}
}
//# end of Functions

//# 
//# Built in commands
//#

//# Sleep
void flatsh_sleep(char **args){
    int t = *args[1];
    sleep(t);}
//# Clear screen
void flatsh_clrs(char **args) {
    printf("%s",CLEAR);
    }
//# List directories
void flatsh_ldirs(char **args) {
    DIR *dir;
    struct dirent *entry;
    if (args[1] != NULL){
        dir = opendir(args[1]);
        if (dir != NULL) {
            while ((entry = readdir(dir)) != NULL) {
		        printf("%s ", entry->d_name);}
        }else{perror("\033[48;2;255;1;1;38;2;0;0;0m Unable to list subdirs\x1B[0m");}}
    if (args[1] == NULL) {
        dir = opendir(".");
        if (dir != NULL) {
            while ((entry = readdir(dir)) != NULL) {
		        printf("%s ", entry->d_name);}
        }else{perror("\033[48;2;255;1;1;38;2;0;0;0m Unable to list subdirs\x1B[0m");}}
    printf("\n",1);
    closedir(dir);}
//# Exit the shell.
void Flatsh_menu_mode(void);
void flatsh_escape(char **args) { Flatsh_menu_mode();}
//# Change working directory.
void flatsh_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("\033[48;2;190;0;0;38;2;0;0;0m fcd\x1B[0m");}}}
//# Make directory.
void flatsh_md(char **args) {
    mode_t permissions = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
     if (mkdir(args[1], permissions) == 0) {
        printf("Dir '%s' created.\n", args[1]);
    } else {
        if (errno == EEXIST) {
            printf("Dir '%s' exists.\n", args[1]);
        } else {fprintf(stderr, "Failed to create %s: %s\n", args[1], strerror(errno));}}}
//# Remove directory.
void flatsh_rd(char **args) {
     if (rmdir(args[1]) == 0) {
        printf("Dir '%s' removed.\n", args[1]);
    } else {
        if (errno == EEXIST) {
            printf("Dir '%s' doesn't exist.\n", args[1]);
        } else {fprintf(stderr, "Failed to remove %s: %s\n", args[1], strerror(errno));}}}
//# Remove file.
void flatsh_rm(char **args) {
     if (remove(args[1]) == 0) {
        printf("File '%s' removed.\n", args[1]);
    } else {
        if (errno == EEXIST) {
            printf("File '%s' doesn't exist.\n", args[1]);
        } else {fprintf(stderr, "Failed to remove %s: %s\n", args[1], strerror(errno));}}}
//# Flatfetch (shell and system info)
//# Most of terminals interpreter custom colors as blue or another colors
//#
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
    printf("%s....._=XYYYX=_.....%s X %s\n",GR,NO, user);
    printf("%s._X&$$$$$$$$$$$&X_.%s X%s \n",GR,NO, str);
    printf("%s&############%s--%s###&%s X Kernel - %s\n",DGR,FLAME,DGR,NO, buffer.sysname);
    printf("%s@#@#@#%s=++%s##%s++-%s#@#@#%s X Release - %s\n",DGR,FLAME,DGR,FLAME,DGR,NO, buffer.release);
    printf("%s@@#@%sEX=%s@#@@%s+++%s#@@#@%s X Machine - %s\n",BK,FLAME,BK,FLAME,BK,NO, buffer.machine);
    printf("%s@@@@%s@@EEEXx==+%s@@@@@%s X Nodename - %s\n",BK,FLAME,BK,NO, buffer.nodename);
    printf("%sX%s@@@@%s@@@@EEX%s@@@@@@%sX%s X Version - %s\n",FLAME,BK,FLAME,BK,FLAME,NO, buffer.version);
    printf("%s=xX&%s@@@@@@@@@@@%s&Xx=%s X FlatShell ver - %s\n",FLAME,BK,FLAME,NO,ver);
    printf("%s...%s-=xX&@@@&Xx=-%s...%s X Total freq * cores - %.2f MHZ * %ld CORES\n",BK,FLAME,BK,NO,max_freq_khz / 1000.0, cores);
    printf("                    X Total Ram Vol - %lld MB\n", total_ram_mb);
    printf("%sX%sX%sX%sX%sX%sX%sX%sX%sX%sX%sX%sX%sX%sX      X\n",BK,DGR,GR,WH,RD,YL,GN,CY,BL,MG,FLAME,ERRcol,MENU,NO);
    free(str);}
//# Print current directory path
void flatsh_pwd(char **args) {
    char cwd[128];
    getcwd(cwd, sizeof(cwd));
    printf("%s\n", cwd);}
//# Print flatshell help text.
void flatsh_help(char **args) {
    printf("%s+-----------------------------------------+-----------------------------------------+%s\n",MENU,NO);
    printf("%s+  It is highly recomended that you will use Text shell mode, menu is still a proto +%s\n",MENU,NO);
    printf("%s+-----------------------------------------+-----------------------------------------+%s\n",MENU,NO);
    printf("%s|FlatSh - Shell with unique ideas        .|Menu mode                               :|%s\n",MENU,NO);
    printf("%s|Text shell mode                         :| Shell          Standart text shell mode.|%s\n",MENU,NO);
    printf("%s| fcd            Change working directory.| Escape         Escape the shell        .|%s\n",MENU,NO);
    printf("%s| fls            List subdirs            .| Help           Show this menu          .|%s\n",MENU,NO);
    printf("%s| fmd            Make subdir             .| Cd             Change working directory.|%s\n",MENU,NO);
    printf("%s| fslp           Sleep for x seconds     .|                                        .|%s\n",MENU,NO);
    printf("%s| frd            Remove subdir           .|                                        .|%s\n",MENU,NO);
    printf("%s| frd            Remove file             .|                                        .|%s\n",MENU,NO);
    printf("%s| unixpygrass/ff Print shell info        .|                                        .|%s\n",MENU,NO);
    printf("%s| quit           Menu mode shell         .|                                        .|%s\n",MENU,NO);
    printf("%s| help           Print this help text    .|                                        .|%s\n",MENU,NO);
    printf("%s| fpwd           Print current dir path  .|                                        .|%s\n",MENU,NO);
    printf("%s| fclear         Clear screen            .|                                        .|%s\n",MENU,NO);
    printf("%s+-----------------------------------------+-----------------------------------------+%s\n",MENU,NO);}
//# end of Built in commands
void Flatsh_menu(void);
struct builtin {
    char *name;
    void (*func)(char **args);};
struct builtin builtins[] = {
    {"help", flatsh_help},
    {"quit", flatsh_escape},
    {"fcd", flatsh_cd},
    {"fmd", flatsh_md},
    {"frd", flatsh_rd},
    {"frm", flatsh_rm},
    {"fls", flatsh_ldirs},
    {"unixpyglass", flatsh_fetch},{"ff", flatsh_fetch},
    {"fslp", flatsh_sleep},
    {"fclear", flatsh_clrs},
    {"fpwd", flatsh_pwd}};
int flatsh_num_builtins() {
    return sizeof(builtins) / sizeof(struct builtin);}
//# 
//# Process/command launcher.
//# 
void flatsh_exec(char **args) {
    for (int i = 0; i < flatsh_num_builtins(); i++) {
        if (strcmp(args[0], builtins[i].name) == 0) {
            builtins[i].func(args);
            return;}}
    pid_t child_pid = fork();
    if (child_pid == 0) {
        execvp(args[0], args);
        perror("flatsh");
        exit(1);}
    else if (child_pid > 0) {
        int status;
        do {
            waitpid(child_pid, &status, WUNTRACED);}
    while (!WIFEXITED(status) && !WIFSIGNALED(status));}
    else {
        perror("\033[48;2;190;0;0;38;2;0;0;0m flatsh\x1B[0m");}}
//# end of command launcher

//# 
//# Input parser.
//# 
char** flatsh_split_line(char *line) {
    int length = 0;
    int capacity = 16;
    char **tokens = malloc(capacity * sizeof(char*));
    if (!tokens) {
        perror("\033[48;2;190;0;0;38;2;0;0;0m flatsh\x1B[0m");
        exit(1);}
    char *delimiters = " \t\r\n";
    char *token = strtok(line, delimiters);
    while (token != NULL) {
        tokens[length] = token;
        length++;
        if (length >= capacity) {
            capacity = (int) (capacity * 1.5);
            tokens = realloc(tokens, capacity * sizeof(char*));
            if (!tokens) {
                perror("\033[48;2;190;0;0;38;2;0;0;0m flatsh\x1B[0m");
                exit(1);}}
        token = strtok(NULL, delimiters);}
    tokens[length] = NULL;
    return tokens;}
//# end of command parser

//# Read a single line of input from stdin. The return value is a string pointer
//# which needs to be freed once we're finished with it.
//#
char* flatsh_read_line() {
    char *line = NULL;
    size_t buflen = 0;
    errno = 0;
    ssize_t strlen = getline(&line,&buflen,stdin);
    if (strlen < 0) {
        if (errno) {
            perror("flatsh");}
        exit(1);}
    return line;}
//# end of line reader

//#
//# Shell modes
//#
//# Text mode
void Flatsh_text_mode(){
    char histlocation[PATH_MAX];
    const char* home_dir = getenv("HOME");
//    snprintf(histlocation, PATH_MAX, "%s/%s", home_dir, ".hist.flat");
//    FILE *hflat = fopen(histlocation,"a+");
//    if (hflat == NULL) {
//        perror("flatsh:Errcolor opening history file");
//        hflat = fopen(histlocation, "w");
//        fclose(hflat);
//        hflat = fopen(histlocation, "a+");}
    flatsh_clrs(0);
    if (access("/bin/toybox",F_OK) == 0){
        printf("Toybox found\n");}
	char *user = getenv("USER");
    while (true) {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        unsigned short width = w.ws_col;
        printf("%s%s # ",NO,user);
        char *line = flatsh_read_line();
        char **tokens = flatsh_split_line(line);
        if (tokens[0] != NULL) {
            flatsh_exec(tokens);}
        free(tokens);
        free(line);}}
//# Menu mode
void Flatsh_menu_mode(void){
    //# init and config
    int num_strings = 1;
    int max_len = 100;
    char **args = malloc(num_strings * sizeof(char *));
    char **nul = malloc(sizeof(char *));
    nul[0] = NULL; 
    int menumax = 4;
    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    //# menu enteries
    char *choices[] = {
        "Help",
        "Shell",
        "Cd",
        "Escape"
    };
    int highlight = 0;
    int choice = 0;
    int c;
    int selposx;
    int selposy;
    //# console width and height in chars
    while(1) {
    int cols = 80;
    int lines = 24;
    #ifdef TIOCGSIZE
        struct ttysize ts;
        ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
        cols = ts.ts_cols;
        lines = ts.ts_lines;
    #elif defined(TIOCGWINSZ)
        struct winsize ts;
        ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
        cols = ts.ws_col;
        lines = ts.ws_row;
    #endif
    //# clear screen and start menu mode (ncurses) 
        clear();
        start_color();
        init_color(109, 500, 500, 500); 
        init_color(19,250,0,0);
        WINDOW *menu = newwin(cols, lines, 0, 0);
        init_pair(1,109, COLOR_RED);
        wbkgd(menu,COLOR_PAIR(1));
        int centrl = (cols/2) - 21;
        mvprintw(2,centrl,"Use <- and -> to navigate, Enter to select:");
        char loc[40];
        getcwd(loc, sizeof(loc));
        mvprintw(1,5,"%s",loc);
        centrl = (cols/2) - 20;
        mvprintw(0,centrl,"+------------| Flatsh menu |------------+");
        for(int i = 0; i < menumax; i++) {
            if(i == highlight) attron(COLOR_PAIR(1));
            centrl = (cols/2) - (strlen(choices[i])/2);
            selposx = 10+(10*i);
            if (selposx>lines-10){};
            mvprintw(5,selposx, "%s", choices[i]);
            attroff(COLOR_PAIR(1));}
        c = getch();
        switch(c) {
            case KEY_LEFT:
                highlight = (highlight == 0) ? menumax - 1 : highlight - 1;
                break;
            case KEY_RIGHT:
                highlight = (highlight == menumax - 1) ? 0 : highlight + 1;
                break;
            case 10:
                choice = highlight;
                if (choice == 0) {
                    //# stop menu mode; go to text mode shell (void Flatsh_text_mode()); reboot menu mode
                    endwin();
                    echo();
                    flatsh_help(nul);
                    initscr();
                    noecho();};
                if (choice == 1) {
                    //# stop menu mode; go to text mode shell (void Flatsh_text_mode()); reboot menu mode
                    endwin();
                    Flatsh_text_mode();
                    refresh();
                    initscr();
                    goto nogetchs ;};
                if (choice == 2) {
                    //# get arguments; stop menu mode; cd to args; reboot menu mode
                    echo();
                    for (int i = 0; i < num_strings; i++) {
                        args[i] = malloc(max_len * sizeof(char));
                        mvprintw(lines-5,0,"Arguments > ");
                        mvgetstr(lines-5,13,args[i]);};
                    if (strlen(args[0])>0){chdir(args[0]);};
                    initscr();
                    noecho();
                    goto nogetchs;};
                if (choice == 3) {
                    //# stop menu mode; exit
                    endwin();free(args);exit(1);};
                refresh();
                centrl = (cols/2) - (strlen("Press any key to return to menu...")/2);
                mvprintw(lines-2, centrl, "Press any key to return to menu...");
                getch();
                nogetchs:
                break;
            default:
                break;}}
end:
    endwin;}
//# end of Shell modes
//#
//# Init
//#
int main() {
    while (1) {Flatsh_menu_mode();}
    return 0;
}

//# end of Shell
