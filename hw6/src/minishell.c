#define _POSIX_C_SOURCE 200809L
#define MAX_LINE 4096
#define MAX_TOKEN 2048
#define MAX_PIPE 64
#include "minishell.h"
#include "parse.h"

volatile sig_atomic_t interrupted = 0; // Tracks if SIGINT was received

void sigint(int sig)
{
    (void)sig;       // Suppress unused parameter warning
    interrupted = 1; // Set the interrupted flag
}

void signalhandler(void)
{
    struct sigaction sa;
    sa.sa_handler = sigint;   // Set the signal handler
    sa.sa_flags = SA_RESTART; // Restart interrupted system calls
    sigemptyset(&sa.sa_mask); // No additional signals are blocked

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        fprintf(stderr, "Error: Cannot register signal handler. %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void clp_writer(void)
{
    char cwd[PATH_MAX]; // Buffer for current working directory

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        fprintf(stderr, "Error: Cannot get current working directory. %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    // Print the formatted directory directly to stdout
    printf("[%s%s%s]$ ", BRIGHT_BLUE, cwd, DEFAULT);
    fflush(stdout); // Flush stdout to ensure the prompt is printed immediately
}

void builtin_exit(int argc, char **argv)
{
    if (argc > 1)
    {
        fprintf(stderr, "Error: exit takes no arguments.\n");
        return; // QUESTION: if exit() improperly called, should we return or exit?
    }
    exit(EXIT_SUCCESS);
}

//
static const char *get_home(void)
{
    // Short note to self about static const char *:
    //  static: only visibile within this file
    //  const: doesn't modify any global variables

    struct passwd *pw = getpwuid(getuid()); // Get the user ID
    if (!pw)
    {
        fprintf(stderr, "Error: Cannot get passwd entry. %s.\n", strerror(errno));
        return NULL;
    }

    // pw is struct from pwd.h that contains user information
    // we want the home directory of the user
    return pw->pw_dir; // Return the home directory
}

static char *tilde_expansion(const char *arg)
{
    if (!arg)
    {
        fprintf(stderr, "Error: NULL argument passed to tilde_expansion.\n");
        return NULL;
    }

    if (arg[0] != '~')
    {
        return strdup(arg); // If the first character is not ~, return a copy of the string
    }
    const char *home = get_home(); // Get the home directory
    if (!home)                     // Check if home is NULL/empty
    {
        fprintf(stderr, "Error: Cannot get to home folder.\n");
        return NULL;
    }

    size_t length = strlen(home) + strlen(arg); // Calculate the length of the new string
    // Why do we to do that? How does strlen handle slashes, tildes, etc.?
    char *output = malloc(length); // Allocate memory for the new string
    if (!output)
    {
        fprintf(stderr, "Error: malloc() failed. %s.\n", strerror(errno));
        return NULL;
    }
    // The +1 skips the tilde character; sprint writes to the output buffer
    snprintf(output, length, "%s%s", home, arg + 1); // Copy the home directory and the rest of the string

    return output; // Return the new string
}

void builtin_cd(int argc, char **argv)
{
    const char *target;    // The target directory to change to
    int free_sentinel = 0; // Flag to indicate if we need to free the target

    // Case 1: No arguments or tilde-only, change to home directory
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "~") == 0))
    {
        target = get_home(); // Get the home directory
        if (!target)
        {
            return;
        }
    }
    // Case 2: Target given
    else if (argc == 2)
    {
        // Check if the argument is quoted
        char *quote_checked = quoted_helper(argv[1]);
        if (!quote_checked)
        {
            return; // Check if processing failed
        }

        // if (quote_checked[0] == '~')
        //{
        target = tilde_expansion(quote_checked); // Check for tilde expansion on processed string
        if (!target)
        {
            fprintf(stderr, "Error: Failed to expand path: %s\n", argv[1]);
            free(quote_checked); // Free the processed string
            return;              // Check if tilde expansion failed
        }
        //}

        free_sentinel = 1; // Set the flag to free the target later
    }
    // Case 3: Too many arguments
    else
    {
        fprintf(stderr, "Error: Too many arguments to cd.\n");
        return;
    }
    // Change to the target directory
    if (chdir(target) == -1)
    {
        fprintf(stderr, "Error: Cannot change directory to '%s'. %s.\n", target, strerror(errno));
        return;
    }

    // if (target != argv[1])
    //     free((void *)target); // Free if we malloc'd;
    //  Do we need (void *)target? —> Yes, to tell compiler we intentionally discard the const qualifer
    if (free_sentinel)
    {
        free((void *)target); // Free the expanded string if we allocated it
    }
}

char *quoted_helper(const char *arg)
{
    size_t length = strlen(arg); // Get the length of the string

    if (length >= 2 && arg[0] == '"' && arg[length - 1] == '"')
    {

        // Check if the line starts and ends with double quotes
        char *output = malloc(length - 1); // Allocate memory for the new string
        if (!output)
        {
            fprintf(stderr, "Error: malloc() failed. %s.\n", strerror(errno));
            return NULL;
        }

        strncpy(output, arg + 1, length - 2); // Copy the string without the quotes
        output[length - 2] = '\0';            // Null-terminate the string
        return output;                        // Return the new string
    }

    if (strchr(arg, '"'))
    {
        // If there is quote in the string but doesn't start and end with it
        fprintf(stderr, "Error: Mismatched or missing quotes in argument: %s\n", arg);
        return NULL;
    }

    // If no quotes, return a copy of the argument
    return strdup(arg);
}

void execute_command(int argc, char **argv)
{
    // Check for too many tokens
    if (argc > MAX_TOKEN)
    {
        fprintf(stderr, "Error: Too many arguments.\n");
        return;
    }

    // Check for command length exceeding limit
    if (strlen(argv[0]) > MAX_LINE)
    {
        fprintf(stderr, "Error: Command too long.\n");
        return;
    }
    pid_t pid = fork();
    if (pid == -1)
    {
        fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));
        return;
    }
    if (pid == 0)
    {
        execvp(argv[0], argv); // Child process executes the command
        fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));
        // fprintf(stderr, "Error: Command not found: %s\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    { // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
    else
    {
        // Fork failed
        perror("Error: Failed to fork");
    }
}

int main(void)
{

    char *line = NULL; // we will use getline() to malloc here
    size_t cap = 0;    // size_t is an unsigned type; getline grows the buffer, we need to track
    ssize_t length;    // the length of the line returned by getline()
    signalhandler();   // Register the SIGINT handler

    // Begin a loop
    for (;;)
    {
        // Display the prompt
        if (!interrupted)
        {
            // printf("minishell$ ");
            fflush(stdout);
        }
        clp_writer(); // Print the command line prompt

        length = getline(&line, &cap, stdin); // Read a line from stdin
        if (length == -1)
        {
            if (feof(stdin))
            {
                putchar('\n');
                break; // End of input
            }
            else
            {
                fprintf(stderr, "Error: Failed to read from stdin. %s.\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        }

        if (length && line[length - 1] == '\n')
        {
            line[length - 1] = '\0'; // Remove the newline character (strip trailing newline)
        }

        if (*line == '\0') // Checks if the line is empty
        {
            continue; // Skip empty lines
        }

        // TODO: Implement the command parsing and execution logic here
        // Tokenizing the input to process commmands and arguments

        char *argv[MAX_TOKEN];                           // Array to hold the arguments
        int argc = line_splitter(line, argv, MAX_TOKEN); // Call function from parse.c

        if (argc == 0)
        {
            continue; // Skip empty lines
        }
        if (strcmp(argv[0], "exit") == 0)
        {
            builtin_exit(argc, argv); // Check for the exit command
        }
        else if (strcmp(argv[0], "cd") == 0)
        {
            builtin_cd(argc, argv); // Check for the cd command
        }
        else
        {
            execute_command(argc, argv); // Execute the command
        }

        // Debug Helper
        // printf("DEBUGGER: You typed: «%s» \n", line);
    }
    free(line);          // Free the allocated memory for the line
    return EXIT_SUCCESS; // Return success
}
