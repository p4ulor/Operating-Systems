#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/wait.h>
#include <unistd.h>

// The size of characters that can be write to the buffer each time
#define BUFFER_SIZE 4096
// The amount of commands that can be chained
#define PIPE_LIMIT 16

/*
 *	Reads from the standart input.
 *	Writes to the buffer.
 */
void read_line(char* buffer) {
    int idx = 0;
    char c;
    while (1) {
        c = getchar();
        while (c == EOF)
            ;
        if (c == '\n') {
            buffer[idx] = '\0';
            return;
        } else {
            buffer[idx] = c;
        }
        idx++;
    }
}

/*
 *	Formats the input array to convert the pipe into a '\0' (string end).
 *	Populates the ouput with the address of command's start.
 *	Important: Escape String building characters like ' and ".
 */
size_t split(char* input, char* output[]) {
    char text = 0;
    output[0] = input;
    size_t count = 0;
    int length = strlen(input);
    if (length > 0) {
        count++;
    }
    int x = 1;
    for (int i = 0; i < length; i++) {
        char c = input[i];
        switch (c) {
            case '"':
                if (text == 0) {
                    text = '"';
                } else if (text == '"') {
                    text = 0;
                }
                break;
            case '\'':
                if (text == 0) {
                    text = '\'';
                } else if (text == '\'') {
                    text = 0;
                }
                break;
            case '|':
                if (text == 0) {
                    input[i] = 0;
                    output[x] = &input[i + 1];
                    x++;
                    count++;
                }
                break;
            case '\n':
                input[i] = 0;
                count++;
                break;
            case '\0':
                input[i] = 0;
                count++;
                break;
        }
    }
    return count;
}

/*
 *   Gets the pointer to the pointer of the beginning of string.
 *   This way is able to change the pointer of the string.
 *   Wich makes sure the string beginning is trimmed as well.
 */
void trim(char** string) {
    char* actual_string = *string;
    size_t length = strlen(actual_string);
    char begin = 0;
    char* n_string = actual_string;
    char* last = actual_string;
    for (int i = 0; i < length; i++) {
        if (!begin && actual_string[i] == ' ') {
            n_string = actual_string + 1;
        }
        if (actual_string[i] != ' ') {
            last = actual_string + i + 1;
            begin = 1;
        }
    }
    size_t execess = strlen(last);
    for (int i = 0; i < execess; i++) {
        last[i] = 0;
    }
    *string = n_string;
}

/*
 *	Intercepts the exit command to leave the shell.
 */
int check_exit(char* string) {
    if (strcasecmp(string, "exit") == 0) {
        return 1;
    }
    return 0;
}

/*
 *  Ignores interrupt signal on main process while the command runs.
 *  That way the commands gets cancelled.
 *  Console is meant to return for prompt state afterwards.
 */
void signal_ignore(int sginal) { return; }

/*
 *   Creates a new string and copies the command name to it.
 *   Note: The new string allocates more memory that must be managed afterwards.
 */
char* command_name(char* command) {
    int i = 0;
    for (; command[i] != ' ' && command[i] != 0; i++)
        ;
    char* cname = malloc(i * sizeof(char));
    for (int j = 0; j < i; j++) {
        cname[j] = command[j];
    }
    return cname;
}

/*
 *   Returns the pointer to the redirect path.
 *   Or 0 if there's no redirect.
 *   Note: Redirect verbose is removed from the command string.
 */
char* check_redirect(char* command) {
    size_t length = strlen(command);
    char text = 0;
    for (int i = 0; i < length; i++) {
        char c = command[i];
        switch (c) {
            case '\'':
                if (text == 0) {
                    text = c;
                }
                if (text == '\'') {
                    text = 0;
                }
                break;
            case '"':
                if (text == 0) {
                    text = c;
                }
                if (text == '"') {
                    text = 0;
                }
                break;
            case '>':
                if (text == 0) {
                    command[i] = 0;
                    return command + i + 1;
                }
                break;
        }
    }
    return 0;
}

/*
 *   Sets output descriptor to be the file selected for redirection.
 */
void redirect(char* path) {
    FILE* file = fopen(path, "w+");
    dup2(fileno(file), 1);
    close(fileno(file));
}

/*
 *   Counts number of words.
 */
int argument_count(char* command) {
    size_t length = strlen(command);
    int argc = 0;
    bool space = true;
    char text = 0;
    for (int i = 0; i < length; i++) {
        char c = command[i];
        switch (c) {
            case '\'':
                if (text == 0) {
                    text = c;
                }
                if (text == '\'') {
                    text = 0;
                }
                break;
            case '"':
                if (text == 0) {
                    text = c;
                }
                if (text == '"') {
                    text = 0;
                }
                break;
        }
        if (space == true) {
            if (command[i] != ' ') {
                space = false;
                argc++;
            }
        } else {
            if (command[i] == ' ') {
                space = true;
            }
        }
    }
    return argc;
}

/*
 *   Indexes the command string into an array.
 *   Manipulates the original string.
 *   Breaking it into multiple string with new pointers.
 *   New pointers are stored on argv.
 */
void command_to_array(char* command, char** argv, int argc) {
    size_t length = strlen(command);
    bool space = true;
    char text = 0;
    for (int i = 0, j = 0; i < length; i++) {
        char c = command[i];
        switch (c) {
            case '\'':
                if (text == 0) {
                    text = c;
                }
                if (text == '\'') {
                    text = 0;
                }
                break;
            case '"':
                if (text == 0) {
                    text = c;
                }
                if (text == '"') {
                    text = 0;
                }
                break;
            case ' ':
                command[i] = 0;
                break;
        }
        if (space == true) {
            if (c != ' ') {
                space = false;
                argv[j++] = command + i;
            }
        } else {
            if (c == ' ') {
                space = true;
            }
        }
    }
    argv[argc] = NULL;
}

/*
 *  Runs the commands in concurrence using pipes to redirect I/O.
 *  Waits for execution of all tasks before returning.
 *	Note: Make sure a redirection must close next pipe input.
 */
int execute(char** commands, int count) {
    // Save stdin and stdout
    int sin = dup(0);
    int sout = dup(1);
    int procs = count;
    // Create I/O file descriptors arrays
    int fis[procs];
    int fos[procs];
    // Set pipe chain begin and end to bind on stdin and stdout
    fis[0] = sin;
    fos[procs - 1] = sout;
    // Create array to store child processes ID's
    pid_t pids[procs];
    // Launch every process (each running a command)
    for (int i = 0; i < procs; i++) {
        // Generate a pipe for inter processes communication
        int fd[2];
        if (i < procs - 1) {
            pipe(fd);
            fis[i + 1] = fd[0];
            fos[i] = fd[1];
        }
        // Start the process
        pid_t pid = fork();
        if (pid == 0) {
            // Link the child to the corresponding I/O
            dup2(fis[i], 0);
            if (i < procs - 1) {
                dup2(fd[1], 1);
            }
            // Run the command
            char* command = commands[i];
            char* cname = command_name(command);
            char* output = check_redirect(command);
            // Redirects the output if a redirect is found (>)
            // The new ouput is a file that match the trimmed text after the
            // sign
            if (output != 0) {
                trim(&output);
                redirect(output);
            }
            int argc = argument_count(command);
            char** argv = malloc((argc + 1) * sizeof(char*));
            command_to_array(command, argv, argc);
            execvp(cname, argv);
        } else {
            // Parent assigns new born process to the array
            pids[i] = pid;
        }
    }
    // Parent waits for all processes to finish
    for (int i = 0; i < procs; i++) {
        int status;
        waitpid(pids[i], &status, 0);
        // Report the exit code if not 0
        if (status != 0) {
            printf("\nProccess %d exited with code %d.\n", pids[i], status);
        }
        // Release the pipe's resources
        close(fis[i]);
        close(fos[i]);
    }
    // Restore stdin and stdout
    dup2(sin, 0);
    dup2(sout, 1);
    // Close stdin and stdout duplicates
    close(sin);
    close(sout);
    return 0;
}

int main() {
    char* buffer = malloc(BUFFER_SIZE * sizeof(char));
    char** commands = malloc(PIPE_LIMIT * sizeof(char*));
    while (1) {
        printf("$ ");
        read_line(buffer);
        int count = split(buffer, commands);
        for (int i = 0; i < count; i++) {
            trim(&commands[i]);
        }
        if (check_exit(commands[0])) {
            exit(0);
        }
        signal(SIGINT, signal_ignore);
        execute(commands, count);
        signal(SIGINT, NULL);
    }
    free(buffer);
    free(commands);
    printf("\nExited.\n");
    return 0;
}
