#include "vga.h"
#include "uart.h"
#include "string.h"
#include "myPrintk.h"

#define NULLSTR "NULL"
#define NULLPTR 0

#define MAX_ARGC 12
#define MAX_STR_LEN 32
#define MAX_BUF_SIZE 400

// the data structure for shell command
typedef struct _command {
    char *name;
    char *help;
    int (*func)(int argc, char *argv[]);
} command;

int help_func(int argc, char *argv[]);
int cmd_func(int argc, char *argv[]);

// command list
command cmds[] = {
    { "cmd", "cmd : List all the available commands.", cmd_func },
    { "help", "help [command] : Display the information of [command].", help_func },
    { NULLSTR, NULLSTR, NULLPTR }
};

// the data structure to handle the parameters of a command
typedef struct _param {
    int argc;
    char *argv[MAX_ARGC];
} param;

// split a string into clips, with ch as the divider
int split_string(const char *str, char ch, char **str_clips) {

    int clip_num = 0;
    int i = 0;

    while (*str++ == ch); // loop to skip all the serial ch at the begining of string
    str--;

    while (*str) {
        if (*str == ch) {
            while (*str++ == ch); // loop to skip all the serial ch
            if (*--str) { // there are still available chars, not '\0'
                str_clips[clip_num++][i] = '\0';
                i = 0;
            }
        }
        else
            str_clips[clip_num][i++] = *str++;
    } 
    str_clips[clip_num++][i] = '\0';

    return clip_num;
}

// search the input cmd in cmd list, and then execute it if we find it 
int shell_cmd_handler(int argc, char *argv[]) {

    if (!myStrlen(argv[0])) // no available input
        return 1;

    for (int i = 0; myStrcmp(NULLSTR, cmds[i].name) != 0; i++) {
        if (!myStrcmp(argv[0], cmds[i].name)) {
            cmds[i].func(argc, argv); // call the func of input cmd
            return 0;
        }
    }

    myPrintk(0x7, "Command \"%s\" not found.\n", argv[0]);
    return 1;
}

// the func of "help"
int help_func(int argc, char *argv[]) {

    if (argc == 1) {
        myPrintk(0x7, "help [command] : Display the information of [command].\n");
        return 0;
    }

    for (int i = 0; myStrcmp(NULLSTR, cmds[i].name) != 0; i++) {
        if (!myStrcmp(argv[1], cmds[i].name)) {
            myPrintk(0x7, "%s\n", cmds[i].help);
            return 0;
        }
    }
    
    myPrintk(0x7, "Command \"%s\" not found.\n", argv[1]);
    return 1;
}

// the func of "cmd"
int cmd_func(int argc, char *argv[]) {

    for (int i = 0; myStrcmp(NULLSTR, cmds[i].name) != 0; i++)
        myPrintk(0x7, "%s\n", cmds[i].help);

    return 0;
}

// get the input from uart, store it in buf and echo it on vga and uart
void get_input_cmd(char *buf) {

    char *str = buf;
    unsigned char input;

    while ((input = uart_get_char()) != 13) {
    handle_input:
        if (input == 13)
            break;

        else if (input == 127) { // backspace
            if (str > buf) {
                str--;
                myPrintk(0x7, "\b \b"); // echo
                uart_put_chars("\b \b");
            }
        }

        else if (input == '\033') { // dierction keys and Esc key
            if ((input = uart_get_char()) == '[') // dierction keys
                input = uart_get_char();
            else // Esc key
                goto handle_input;
        }

        else {
            *str++ = input;
            myPrintk(0x7, "%c", input); // echo
            uart_put_char(input);
        }
    }
    *str = '\0';

    myPrintk(0x7, "\n"); // echo
    uart_put_chars("\r\n");
}


void startShell(void) {

    char buf[MAX_BUF_SIZE]; // input buffer
    char cmd_params[MAX_ARGC][MAX_STR_LEN]; // to store the parameters of cmd
    param cmd; // argc and *argv[] of cmd
    for (int i = 0; i < MAX_ARGC; i++)
        cmd.argv[i] = cmd_params[i]; // let the argv point to cmd_params

    while (1) {

        myPrintk(0xa, "shell@yjwOS:");
        myPrintk(0x7, "$ ");

        get_input_cmd(buf);

        cmd.argc = split_string(buf, ' ', cmd.argv); // split the input with space
        
        shell_cmd_handler(cmd.argc, cmd.argv);
    }

    return;
}
