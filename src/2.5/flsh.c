//# 
//# Ready to compile programm
//# IF YOU DON'T KNOW WHAT YOU'RE DOING: DO NOT EDIT.
//# gcc -static -lncurses [filename] -o [compiled filename] to compile. 
//# FlatShell source for UNIX.
//#
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <locale.h>
#include <ncurses.h>
#include <signal.h>
#include <pwd.h>
#include <errno.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/wait.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
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
        else if (num = 1){mvprintw(3,centrl,"%s",text);}}}
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
		        printf("%s    ", entry->d_name);}
        }else{perror("\033[48;2;255;1;1;38;2;0;0;0m Unable to list subdirs\x1B[0m");}}
    if (args[1] == NULL) {
        dir = opendir(".");
        if (dir != NULL) {
            while ((entry = readdir(dir)) != NULL) {
		        printf("%s    ", entry->d_name);}
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
void flatsh_unixpyglass(char **args) {
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
    char *ver = "Shell 2.0 + Menu 1.0" ;
    char *home = getenv("HOME");
    char x = 'x';
    int num = strlen(home) + 2 ;
    char* str = (char*)malloc(num + 3);
    memset(str, x, num); 
    str[num] = '\0'; 
//# show info
    printf("%s....._=XYYYX=_.....%s X %s\n",GR,NO, home);
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
//# embedderbsdmicroproto
void Bsd_micro(void);
//# Print flatshell help text.
void flatsh_help(char **args) {
    setlocale(LC_ALL, "");
    printf("%s╔═════════════════════════════════════════════════════════════════════════════════╗%s\n",MENU,NO);
    printf("%s║ This shell doesnt support most of popular features, don\'t run any piped         ║%s\n",MENU,NO);
    printf("%s║ or OP programs, it could harm your system, or make it unstable,                 ║%s\n",MENU,NO);
    printf("%s║ This shell was designed to provide easier usage for new Linux/BSD users.        ║%s\n",MENU,NO);
    printf("%s╠════════════════════════════════════════╦════════════════════════════════════════╣%s\n",MENU,NO);
    printf("%s║FlatSh - Shell with unique ideas        ║ fpwd           Print current dir path  ║%s\n",MENU,NO);
    printf("%s║Text shell mode help                   :║ fls            List subdirs            ║%s\n",MENU,NO);
    printf("%s║ fcd            Change working directory║ Escape         Escape the shell        ║%s\n",MENU,NO);
    printf("%s║ fmd            Make subdir             ║ Help           Show this menu          ║%s\n",MENU,NO);
    printf("%s║ fclear         Clear screen            ║ Cd             Change working directory║%s\n",MENU,NO);
    printf("%s║ fslp           Sleep for x seconds     ║ help           Print this help text    ║%s\n",MENU,NO);
    printf("%s║ frm            Remove file             ║ menu           Menu mode shell         ║%s\n",MENU,NO);
    printf("%s║ unixpyglass/ff Print shell info        ║                                        ║%s\n",MENU,NO);
    printf("%s╚════════════════════════════════════════╩════════════════════════════════════════╝%s\n",MENU,NO);}
//# end of Built in commands
void Flatsh_menu(void);
struct builtin {
    char *name;
    void (*func)(char **args);};
struct builtin builtins[] = {
    {"help", flatsh_help},
    {"menu", flatsh_escape},
    {"fcd", flatsh_cd},
    {"fmd", flatsh_md},
    {"frd", flatsh_rd},
    {"frm", flatsh_rm},
    {"fls", flatsh_ldirs},
    {"unixpyglass", flatsh_unixpyglass},{"ff", flatsh_unixpyglass},
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
    char configl[PATH_MAX];
    const char* home_dir = getenv("HOME");
    struct passwd *pw = getpwuid(geteuid());
//    snprintf(configl, PATH_MAX, "%s/%s", home_dir, ".hist.flat");
//    FILE *hflat = fopen(configl,"a+");
//    if (hflat == NULL) {
//        perror("flatsh:Errcolor opening history file");
//        hflat = fopen(configl, "w");
//        fclose(hflat);
//        hflat = fopen(configl, "a+");}
    flatsh_clrs(0);
    if (access("/bin/toybox",F_OK) == 0){
        printf("Toybox found\n");}
    if (access("/bin/busybox",F_OK) == 0){
        printf("Busybox found\n");}
	char *user = getenv("USER");
    while (true) {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        unsigned short width = w.ws_col;
        if (pw) {printf("%s%s # ",NO,pw->pw_name);}
        else {printf("%s%s # ",NO,home_dir);}
        char *line = flatsh_read_line();
        char **tokens = flatsh_split_line(line);
        if (tokens[0] != NULL) {
            flatsh_exec(tokens);}
        free(tokens);
        free(line);}}
//# Menu mode
void Flatsh_config_mode(void);
void Flatsh_menu_mode(void){
    //# init and config
    char confthm[PATH_MAX];
    char confbord[PATH_MAX];
    const char* home_dir = getenv("HOME");
    snprintf(confthm, PATH_MAX, "%s/.flatcfg/thm", home_dir);
    FILE *thm = fopen(confthm,"r");
    snprintf(confbord, PATH_MAX, "%s/.flatcfg/bor", home_dir);
    FILE *bor = fopen(confbord,"r");
    setlocale(LC_ALL, "");
    int num_strings = 1;
    int max_len = 100;
    char **args = malloc(num_strings * sizeof(char *));
    char **nul = malloc(sizeof(char *));
    char *bc1,*bc2,*bc3,*bc4,*btb,*brl; 
    mode_t ruleset = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
    nul[0] = NULL; 
    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    set_escdelay(5);
    //# menu enteries
    char *choices[] = {
        "F1  ShellHelp  Show available shell builtin commands",
        "s   Shell      Switch in to text shell mode         ",
        "f   UXpyglass  Show system info                     ",
        "    ListSubd   List subdirs in current dir          ",
        "    ChangeDir  Change current working dir           ",
        "    MakeDir    Make a new directory                 ",
        "BSP Remove     Remove existing file or dir          ",
        "    Rename     Rename existing file or dir          ",
        "    Sleep      Stop any input for x seconds         ",
        "TAB Configure  Configure this shell                 ",
        "ESC Escape     Escape from the shell                "
    };
    int menumax=sizeof(choices)/sizeof(choices[0]);
    int highlight=0;
    int choice=0;
    int c;
    int thmf = fgetc(thm) -48;
    int borf = fgetc(bor) -48;
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
        if (can_change_color()== TRUE) {
            init_color(COLOR_YELLOW, 650, 650, 650); 
            init_color(COLOR_RED,305,0,0);};
        if (thmf==0){
            init_pair(1,COLOR_WHITE,COLOR_RED);
            init_pair(2,COLOR_RED,COLOR_YELLOW);}
        else if (thmf==1){
            init_pair(1,COLOR_YELLOW,COLOR_BLUE);
            init_pair(2,COLOR_BLUE,COLOR_YELLOW);}
        else if (thmf==2){
            init_pair(1,COLOR_YELLOW,COLOR_BLACK);
            init_pair(2,COLOR_BLACK,COLOR_YELLOW);}
        if (borf==0){
            bc1="\u2554";bc2="\u2557";bc3="\u255A";bc4="\u255D";btb="\u2550";brl="\u2551";}
        else if (borf==1){
            bc1=bc2=bc3=bc4="#";btb="=";brl="|";}
        else if (borf==2){
            bc1=bc2=bc3=bc4=btb=brl="/";}
        bkgd(COLOR_PAIR(1));
        int centrl = (cols/2) - 26;
        mvprintw(2,centrl,"Use up and down arrows to navigate, Enter to select:");
        char loc[40];
        curs_set(0);
        getcwd(loc, sizeof(loc));
        mvprintw(1,cols-10-strlen(loc),"%s",loc);
        char *user = getenv("HOME");
        mvprintw(1,10,"%s",user);
        attron(COLOR_PAIR(2));
        mvaddstr(0, 0, bc1);
        for (int x = 1; x < cols-1; x++) {mvaddstr(0,x,btb);};
        mvaddstr(lines-1, 0, bc3);
        for (int x = 1; x < cols-1; x++) {mvaddstr(lines-1,x,btb);};
        mvaddstr(0, cols-1, bc2);
        mvaddstr(lines-1, cols-1, bc4);
        for (int x = 1; x < lines-1; x++) {mvaddstr(x,0,brl);};
        for (int x = 1; x < lines-1; x++) {mvaddstr(x,cols-1,brl);};
        centrl = (cols/2) - 7;
        mvprintw(0,centrl,"# Flatsh menu #");
        int dynpos;
        attroff(COLOR_PAIR(2));
        for(int i = 0; i < menumax; i++) {
            if(i == highlight) attron(COLOR_PAIR(2));
            mvprintw(i+4,5,"%s",choices[i]);
            attroff(COLOR_PAIR(1));}
        c = getch();
        switch(c) {
            case ('c')&0x1f:
                attron(COLOR_PAIR(2));
                mvprintw(lines-2,4,"EXIT using Ctrl+C is not supported");
                attroff(COLOR_PAIR(2));
            case KEY_F(1):
                highlight = 0;
                break;
            case 's':
                highlight = 1;
                break;
            case 'f':
                highlight = 2;
                break;
            case KEY_BACKSPACE:
                highlight = 6;
                break;
            case TAB:
                highlight = menumax-2;
                break;
            case ESC:
                highlight = menumax-1;
                break;
            case KEY_UP:
                highlight = (highlight == 0) ? menumax - 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == menumax - 1) ? 0 : highlight + 1;
                break;
            case 10:
                choice = highlight;
                if (choice == 0) {
                    endwin();
                    flatsh_clrs(nul);
                    flatsh_help(nul);
                    printf("Press Enter to return to menu...");
                    getchar();
                    flatsh_clrs(nul);
                    initscr();}
                else if (choice == 1) {
                    endwin();
                    flatsh_clrs(nul);
                    Flatsh_text_mode();
                    flatsh_clrs(nul);
                    initscr();}
                else if (choice == 2) {
                    endwin();
                    flatsh_clrs(nul);
                    flatsh_unixpyglass(nul);
                    printf("Press Enter to return to menu...");
                    getchar();
                    flatsh_clrs(nul);
                    initscr();};
                    if (choice == 3) {
                    endwin();
                    flatsh_clrs(nul);
                    flatsh_ldirs(nul);
                    printf("Press Enter to return to menu...");
                    getchar();
                    flatsh_clrs(nul);
                    initscr();}
                else if (choice == 4) {
                    echo();
                    for (int i = 0; i < num_strings; i++) {
                        args[i] = malloc(max_len * sizeof(char));
                        mvprintw(lines-2,2,"Dir name > ");
                        mvgetstr(lines-2,13,args[i]);};
                    if (strlen(args[0])>0){chdir(args[0]);}
                    initscr();
                    noecho();}
                else if (choice == 5) {
                    echo();
                    for (int i = 0; i < num_strings; i++) {
                        args[i] = malloc(max_len * sizeof(char));
                        mvprintw(lines-2,2,"Dir name > ");
                        mvgetstr(lines-2,13,args[i]);};
                    if (strlen(args[0])>0){mkdir(args[0],ruleset);}
                    noecho();}
                else if (choice == 6) {
                    echo();
                    for (int i = 0; i < num_strings; i++) {
                        args[i] = malloc(max_len * sizeof(char));
                        mvprintw(lines-2,2,"File name > ");
                        mvgetstr(lines-2,14,args[i]);};
                    if (strlen(args[0])>0){remove(args[0]);};
                    noecho();}
                else if (choice == 7) {
                    echo();
                    for (int i = 0; i < num_strings; i++) {
                        args[i] = malloc(max_len * sizeof(char));
                        refresh();
                        mvprintw(lines-3,2,"File name1 > ");
                        mvprintw(lines-2,2,"File name2 > ");
                        mvgetstr(lines-3,15,args[0]);
                        mvgetstr(lines-2,15,args[1]);};
                    if (strlen(args[0])>0,strlen(args[1])>0){rename(args[0],args[1]);};
                    noecho();}
                else if (choice == 8) {
                    echo();
                    for (int i = 0; i < num_strings; i++) {
                        args[i] = malloc(max_len * sizeof(char));
                        mvprintw(lines-2,2,"Sleep > ");
                        mvgetstr(lines-2,18,args[i]);};
                        mvprintw(lines-2,2,"Sleeping ");
                        int t = *args[0];
                    if (strlen(args[0])>0){sleep(t);};
                    noecho();}
                else if (choice == menumax-2) {
                    Flatsh_config_mode();}
                else if (choice == menumax-1) {
                    endwin();free(args);flatsh_clrs(nul);exit(1);};
                refresh();
                break;
            default:
                break;}}
end:
    endwin;}
//# Config mode
void Flatsh_config_mode(void){
    //# init and config
    const char* home_dir = getenv("HOME");
    char fthm[PATH_MAX];
    snprintf(fthm, PATH_MAX, "%s/.flatcfg/thm", home_dir);
    char bor[PATH_MAX];
    snprintf(bor, PATH_MAX, "%s/.flatcfg/bor", home_dir);
    setlocale(LC_ALL, "");
    int num_strings = 1;
    int max_len = 100;
    char **args = malloc(num_strings * sizeof(char *));
    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    set_escdelay(5);
    //# menu enteries
    char *choices[] = {
        "a: Color scheme           ",
        "b: Borders type           ",
        "c: Read installation guide",
        "x: Finished configuring   "
    };
    char *guide[] = {
        "If you want to access this shell from everywhere, you need to install this shell",
        "into $PATH, just so you know, you can access $PATH variable by using",
        "   echo $PATH             ",
        "If you get output (it should look like /dir1:/dir2) you can select one of them",
        "and copy this program into one of them, i will copy into the most basic",
        "   sudo cp ./FlSh /bin",
        "The last thing you can do is change access for users ",
        "   sudo chmod a+x /bin/FlSh",
        "",
        "To exit from guide press any button."};
    int menumax=sizeof(choices)/sizeof(choices[0]);
    int guidemax=sizeof(guide)/sizeof(guide[0]);
    int highlight=0;
    int choice=0;
    int c;
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
        if (can_change_color()== TRUE) {
            init_color(COLOR_WHITE, 650, 650, 650); }
        init_pair(1,COLOR_WHITE,COLOR_BLUE);
        init_pair(2,COLOR_BLUE,COLOR_WHITE);
        bkgd(COLOR_PAIR(1));
        int centrl = (cols/2) - 15;
        mvprintw(1,centrl,"Restart shell to apply changes");
        centrl = (cols/2) - 26;
        mvprintw(2,centrl,"Use up and down arrows to navigate, Enter to select:");
        char loc[40];
        curs_set(0);
        const char *cornr = "#";
        attron(A_BOLD);
        attron(COLOR_PAIR(2));
        mvaddstr(0, 0, cornr);
        for (int x = 1; x < cols-1; x++) {mvaddstr(0,x,"=");};
        mvaddstr(lines-1, 0, cornr);
        for (int x = 1; x < cols-1; x++) {mvaddstr(lines-1,x,"=");};
        mvaddstr(0, cols-1, cornr);
        mvaddstr(lines-1, cols-1, cornr);
        for (int x = 1; x < lines-1; x++) {mvaddstr(x,0,"|");};
        for (int x = 1; x < lines-1; x++) {mvaddstr(x,cols-1,"|");};
        centrl = (cols/2) - 7;
        mvprintw(0,centrl,"# Config menu #");
        int dynpos;
        attroff(COLOR_PAIR(2));
        attroff(A_BOLD); 
        for(int i = 0; i < menumax; i++) {
            if(i == highlight) attron(COLOR_PAIR(2));
            mvprintw(i+4,5,"%s",choices[i]);
            attroff(COLOR_PAIR(1));}
        c = getch();
        switch(c) {
            case 'a':
                highlight = 0;
                break;
            case 'b':
                highlight = 1;
                break;
            case 'c':
                highlight = menumax-2;
                break;
            case 'x':
                highlight = menumax-1;
                break;
            case KEY_UP:
                highlight = (highlight == 0) ? menumax - 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == menumax - 1) ? 0 : highlight + 1;
                break;
            case 10:
                choice = highlight;
                if (choice == 0) {
                    echo();
                    args[0] = malloc(max_len * sizeof(char));
                    FILE *thm = fopen(fthm,"w+");
                    mvprintw(lines-3,2,"FlatOS(0),NetCFG(1),Black&Gray(2)");
                    mvprintw(lines-2,2,"Theme > ");
                    mvgetstr(lines-2,9,args[0]);
                    fprintf(thm, "%s", args[0]);
                    fclose(thm);
                    noecho();}
                else if (choice == 1) {
                    echo();
                    args[0] = malloc(max_len * sizeof(char));
                    FILE *borf = fopen(bor,"w+");
                    mvprintw(lines-3,2,"CP437(0),ASCII(1),S/ASH(2)");
                    mvprintw(lines-2,2,"Border > ");
                    mvgetstr(lines-2,10,args[0]);
                    fprintf(borf, "%s", args[0]);
                    fclose(borf);
                    noecho();}
                else if (choice==menumax-2){
                    for(int g = 0; g < guidemax; g++) {
                        mvprintw(g+4,5,"%s",guide[g]);}
                        getch();}
                else if (choice == menumax-1) {
                    goto end;}
                refresh();
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
    RESET:
    signal(SIGINT, SIG_IGN);
    mode_t ruleset = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
    char line[10];
    char thmp[PATH_MAX];
    char borp[PATH_MAX];
    char configd[PATH_MAX];
    const char* home_dir = getenv("HOME");
    snprintf(configd, PATH_MAX, "%s/.flatcfg", home_dir);
    snprintf(thmp, PATH_MAX, "%s/.flatcfg/thm", home_dir);
    snprintf(borp, PATH_MAX, "%s/.flatcfg/bor", home_dir);
    FILE *cthm = fopen(thmp,"r+");
    FILE *cbor = fopen(borp,"r+");
    if (access(configd,F_OK) != 0) {
        if (errno==ENOENT){mkdir(configd,ruleset);}
        if (errno==ENOTDIR){perror("Could not access dir");}}
    if (cthm == NULL) {
        perror("flatsh:couldn\'t open themes file");
        FILE *cthm = fopen(thmp, "w");
        fclose(cthm);}
    if (cbor == NULL) {
        perror("flatsh:couldn\'t open borders file");
        FILE *cbor = fopen(borp, "w");
        fclose(cbor);}
    if (fgets(line, sizeof(line), cthm)==NULL){
        perror("flatsh:couldn\'t find theme file contents\nPress enter to continue to config mode");
        getchar();
        Flatsh_config_mode();}
    if (fgets(line, sizeof(line), cbor)==NULL){
        perror("flatsh:couldn\'t find theme file contents\nPress enter to continue to config mode");
        getchar();
        Flatsh_config_mode();}
    while (1) {Flatsh_menu_mode();}
    fclose(cthm);
    fclose(cbor);
    return 0;
}
//# end of Shell
