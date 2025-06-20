#define _POSIX_C_SOURCE 200809L
#include <stdio.h>     // For printf, fprintf
#include <stdlib.h>    // For EXIT_SUCCESS, EXIT_FAILURE
#include <unistd.h>    // For getopt and optarg
#include <sys/types.h> // For fork
#include <sys/wait.h>  // For waitpid
#include <string.h>    // For strcmp
#include <errno.h>     // For errno

#define BUFFER_SIZE 4096 // For buffer size, can be adjusted as needed

int main(int argc, char *argv[])
{
    // Check if the correct number of arguments is provided
    // and if the options are in the correct order
    // i.e., ./spfind -d <directory> -p <permissions string>
    if (argc != 5 || strcmp(argv[1], "-d") != 0 || strcmp(argv[3], "-p") != 0)
    {
        fprintf(stderr, "Usage: ./spfind -d <directory> -p <permissions string>\n");
        return EXIT_FAILURE;
    }

    int pps[2]; // integer array of size 2 for the pipe between pfind and sort (pps: pipe pfind sort)
    int psp[2]; // integer array of size 2 for the pipe between sort and parent (psp: pipe sort parent)

    if (pipe(pps) == -1) // check if pipe creation failed
    {
        perror("pipe");
        return EXIT_FAILURE;
    }

    if (pipe(psp) == -1)
    {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t pid_pfind = fork(); // Create a child process, check if fork failed
    if (pid_pfind < 0)
    {
        perror("fork");
        return EXIT_FAILURE;
    }
    else if (pid_pfind == 0) // if successful, fork returns 0 to the child process
    {
        // this child process will run pfind
        close(pps[0]);               // Close unused read end
        dup2(pps[1], STDOUT_FILENO); // Redirect stdout to the write end of the pipe
        close(pps[1]);               // Close the write end of the pipe

        close(psp[0]); // Close unused read end
        close(psp[1]); // Close unused write end

        execl("./pfind", "pfind", "-d", argv[2], "-p", argv[4], NULL); // Execute pfind
        // Note: execl does not return on success, so everything below should not be reached
        fprintf(stderr, "Error: pfind failed.\n"); // If execl fails, print error message
        exit(EXIT_FAILURE);
    }

    pid_t pid_sort = fork();
    if (pid_sort < 0)
    {
        perror("fork");
        return EXIT_FAILURE;
    }
    else if (pid_sort == 0)
    {
        // this child process will run sort
        close(pps[1]);              // Close unused write end
        dup2(pps[0], STDIN_FILENO); // Redirect stdin to the read end of the pipe
        // This allows sort to read the output of pfind
        close(pps[0]); // Close the read end of the pipe

        close(psp[0]);               // Close unused read end
        dup2(psp[1], STDOUT_FILENO); // Redirect stdout to the write end of the pipe
        close(psp[1]);               // Close the write end of the pipe

        execlp("sort", "sort", NULL); // Execute sort via execlp
        // Again, execlp does not return on success, so everything below should not be reached
        // unless execlp fails
        fprintf(stderr, "Error: sort failed.\n");
        exit(EXIT_FAILURE);
    }

    // Parent process
    close(pps[0]); // Close unused read end
    close(pps[1]); // Close unused write end
    close(psp[1]); // Close unused write end

    char buffer[BUFFER_SIZE]; // Set to prevent overload (Don't want to blow the server)
    ssize_t bytes_read;       // Keep track of bytes read from the pipe, ssize_t is signed size_t
    // Note to self: ssize_t is used for read() to handle negative return values
    int line_count = 0; // Keep track of lines by counting newlines (\n)

    while ((bytes_read = read(psp[0], buffer, BUFFER_SIZE)) > 0)
    {
        for (ssize_t i = 0; i < bytes_read; ++i)
        {
            putchar(buffer[i]); // putchar() because we want to print the output of sort letter by letter
            if (buffer[i] == '\n')
            {
                ++line_count;
            }
        }
    }

    if (bytes_read < 0) // Check if read() failed
    {
        perror("read");
        return EXIT_FAILURE; // If read() fails, exit with failure
    }

    close(psp[0]); // Close the read end of the pipe

    // Wait for both children?
    // NOTE: This is important to prevent zombie processes
    // and to ensure that the parent process does not exit before the children
    // are done executing
    waitpid(pid_pfind, NULL, 0);
    waitpid(pid_sort, NULL, 0);

    printf("Total matches: %d\n", line_count);

    return EXIT_SUCCESS;
}
