#include "../myOS/userInterface.h"

#define NULL (void *)0

#define MAX_ARGC 12
#define MAX_STR_LEN 64
#define MAX_BUF_SIZE 800

// the data structure for shell command
typedef struct _command {
    unsigned char *cmd;
    int (*func)(int argc, unsigned char **argv);
    void (*help_func)(void);
    unsigned char *desc;
    struct _command *next;
} command;

command *cmds = NULL; // command list
int exit; // the flag to exit

// the data structure to handle the parameters of a command
typedef struct _param {
    int argc;
    unsigned char *argv[MAX_ARGC];
} param;

// add a new command to cmd list, using malloc to implement
void addNewCmd(unsigned char *cmd, 
		int (*func)(int argc, unsigned char **argv), 
		void (*help_func)(void), 
		unsigned char* desc) {
    
    command *newCmd = (command *)malloc(sizeof(command));
    char *cmd_new = (char *)malloc(myStrlen(cmd) + 1);
    char *desc_new = (char *)malloc(myStrlen(desc) + 1);
    if (!newCmd || !cmd_new || !desc_new)
        return;

    newCmd->cmd = cmd_new;
    newCmd->desc = desc_new;
    newCmd->func = func;
    newCmd->help_func = help_func;
    myStrcpy(newCmd->cmd, cmd);
    myStrcpy(newCmd->desc, desc);

    // insert to the head of list
    newCmd->next = cmds;
    cmds = newCmd;
}

// split a string into clips, with ch as the divider
int split_string(const char *str, char ch, unsigned char **str_clips) {

    int clip_num = 0;
    int i = 0;

    while (*str++ == ch); // loop to skip all the serial ch at the begining of string
    str--;

    while (*str) {
        if (*str == ch) {
            while (*str++ == ch); // loop to skip all the serial ch
            if (*--str) { // there is still available chars, no '\0'
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
int shell_cmd_handler(int argc, unsigned char **argv) {

    if (!myStrlen(argv[0])) // no available input
        return 1;

    command *temp = cmds;
    while (temp) { // find the cmd
        if (myStrcmp(argv[0], temp->cmd) == 0) {
            temp->func(argc, argv); // call the func of input cmd
            return 0;
        }
        temp = temp->next;
    }

    myPrintf(0x7, "Command \"%s\" not found.\n", argv[0]);
    return 1;
}

// the help func of "help"
void help_help(void) {
    myPrintf(0x7, "help [command] : Display the information of [command].\n");
}

// the func of "help"
int help_func(int argc, unsigned char **argv) {

    if (argc == 1) {
        help_help();
        return 0;
    }

    if (argc > 2) {
        myPrintf(0x7, "Wrong number of parameters for command \"help\"!\n");
        return 0;
    }

    command *temp = cmds;
    while (temp) { // find the cmd
        if (!myStrcmp(argv[1], temp->cmd)) {
            myPrintf(0x7, "%s\n", temp->desc);
            return 0;
        }
        temp = temp->next;
    }
    
    myPrintf(0x7, "Command \"%s\" not found.\n", argv[1]);
    return 1;
}

// the func of "cmd"
int cmd_func(int argc, unsigned char **argv) {

    if (argc > 1) {
        myPrintf(0x7, "Wrong number of parameters for command \"cmd\"!\n");
        return 1;
    }

    command *temp = cmds;
    while (temp) {
        myPrintf(0x7, "%s\n", temp->desc);
        temp = temp->next;
    }

    return 0;
}

// the func of "exit"
int exit_func(int argc, unsigned char **argv) {
    exit = 1;
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
                myPrintf(0x7, "\b \b"); // echo
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
            myPrintf(0x7, "%c", input); // echo
        }
    }
    *str = '\0';

    myPrintf(0x7, "\n"); // echo
}

// register the shell commands
void initShell(void) {
    addNewCmd("cmd", cmd_func, NULL, "cmd : List all the available commands.");
    addNewCmd("help", help_func, help_help, "help [command] : Display the information of [command].");
    addNewCmd("exit", exit_func, NULL, "exit : Exit shell and shut down the OS.");
}

void startShell(void) {

    char buf[MAX_BUF_SIZE]; // input buffer
    char cmd_params[MAX_ARGC][MAX_STR_LEN]; // to store the parameters of cmd
    param cmd; // argc and *argv[] of cmd
    for (int i = 0; i < MAX_ARGC; i++)
        cmd.argv[i] = cmd_params[i]; // let the argv point to cmd_params
    myPrintf(0x7, "\n");
    exit = 0;

    while (!exit) {

        myPrintf(0x2, "shell@yjwOS:");
        myPrintf(0x7, "$ ");

        get_input_cmd(buf);

        cmd.argc = split_string(buf, ' ', cmd.argv); // split the input with space
        
        shell_cmd_handler(cmd.argc, cmd.argv);
    }

}
