#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include "util.h"

int client_socket = -1;
char username[MAX_NAME_LEN + 1];
char inbuf[BUFLEN + 1];
char outbuf[MAX_MSG_LEN + 1];

void handle_sigint(int sig)
{
    // Clear buffers
    memset(outbuf, '\0', sizeof(outbuf));
    memset(inbuf, '\0', sizeof(inbuf));

    // Send a termination message to the server
    if (client_socket != -1)
    {
        // important to send "bye" to the server if possible
        // it runs the cleanup code frees up usernames, etc.
        const char *termination_message = "bye";
        // Send the termination message to the server and check if it was successful
        if (send_with_length(client_socket, termination_message, strlen(termination_message) + 1) == -1)
        {
            // if not successful, print error message
            fprintf(stderr, "Error: Failed to send termination message to server. %s\n", strerror(errno));
        }
    }

    // Even if we don't successfully send the termination message, we still want to close the socket
    // if it is still open and exit the program gracefully
    // Close the socket
    if (client_socket != -1)
    {
        close(client_socket);
    }

    printf("\nSIGINT received. Closed and flushed.\n");
    exit(EXIT_SUCCESS);
}

int handle_stdin()
{
    // Quite similar to part 2 in the main
    if (fgets(outbuf, sizeof(outbuf), stdin) == NULL)
    {
        if (feof(stdin))
        {
            // Detect if end of input stream (EOF)
            printf("End of input detected. Exiting.\n");
            close(client_socket); // Do I need to close?
            exit(EXIT_SUCCESS);
        }
        else
        {
            fprintf(stderr, "Error: Failed to read from stdin.\n");
            return -1; // Error
        }
    }
    // intial check
    // else
    // {
    //    fprintf(stderr, "DEBUG: Read from stdin: %s\n", outbuf);
    // }

    size_t len = strlen(outbuf);

    // Check if the message is too long
    if (len >= MAX_MSG_LEN)
    {
        fprintf(stderr, "Sorry, limit your message to 1 line of at most %d characters.\n", MAX_MSG_LEN);

        // Consume the rest of the input until EOF or newline
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF)
            ;

        memset(outbuf, '\0', sizeof(outbuf)); // Clear the buffer

        return 0; // Discard the message and return
    }

    if (len > 0 && outbuf[len - 1] == '\n')
    {
        outbuf[len - 1] = '\0'; // Remove newline character, null-terminate
        len--;                  // Decrease length by 1
    }
    // include the null terminator in the length
    len++;

    // Ignore blank messages
    if (len == 0)
    {
        memset(outbuf, '\0', sizeof(outbuf)); // Clear the buffer
        return 0;                             // Nothing to send
    }

    // Only need to check if the length/size is correct
    // len_network ensures exactly 2 bytes are sent
    if (send_with_length(client_socket, outbuf, len) == -1)
    {
        fprintf(stderr, "Error: Failed to send message to server. %s.\n", strerror(errno));
        memset(outbuf, '\0', sizeof(outbuf)); // Clear the buffer
        return -1;
    }

    // Handle "bye" message
    if (strcmp(outbuf, "bye") == 0)
    {
        printf("Goodbye.\n");
        close(client_socket);
        exit(EXIT_SUCCESS); // Exit the program
    }

    memset(outbuf, '\0', sizeof(outbuf)); // Clear the buffer
    return 0;                             // Success
}

int handle_client_socket()
{
    // Did I handle this in the wrong place?
    int bytes_received = recv_with_length(client_socket, inbuf, sizeof(inbuf));
    if (bytes_received == 0)
    {
        fprintf(stderr, "Error: Server closed the connection.\n");
        return -1;
    }
    else if (bytes_received == -1)
    {
        // If an error occurred and it wasn't interrupted by a signal
        if (errno != EINTR)
        {
            fprintf(stderr, "Warning: Failed to receive incoming message.\n");
        }
        return -1; // Return to allow the program to continue
    }

    inbuf[bytes_received] = '\0'; // Null-terminate the received message
    printf("%s\n", inbuf);

    // Handle "bye" message from the server
    if (strcmp(inbuf, "bye") == 0)
    {
        printf("\nServer initiated shutdown.\n");
        close(client_socket);
        exit(EXIT_SUCCESS);
    }

    return 0;
}

int main(int argc, char **argv)
{
    // Check if number of arguments is correct
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <server IP> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Register the SIGINT handler
    signal(SIGINT, handle_sigint);

    struct sockaddr_in sa; // Server address
    // inet_pton() checks if the address is valid
    // including whether a, b, c, d are in the range 0-255
    // returns 1 if valid, 0 if not, or -1 if address family is not supported
    if (inet_pton(AF_INET, argv[1], &(sa.sin_addr)) != 1)
    {
        fprintf(stderr, "Error: Invalid address '%s'.\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Output success case for integer for testing server address
    if (inet_pton(AF_INET, argv[1], &(sa.sin_addr)) == 1)
    {
        fprintf(stdout, "SUCCESS: '%s'.\n", argv[1]);
    }

    int port;
    // Could use is_integer() to check if port number given is an integer
    if (!parse_int(argv[2], &port, "port")) // Check port using parse_int, which assigns if valid
    {
        fprintf(stderr, "Error: Invalid port '%s'. Must been an integer\n", argv[2]);
        exit(EXIT_FAILURE);
    }
    if (port < 1024 || port > 65535) // Check port within range
    {
        fprintf(stderr, "Error: Port '%d' out of range [1024, 65535].\n", port);
        exit(EXIT_FAILURE);
    }

    // username loop - using while(1) and break to exit
    while (1)
    {
        printf("Enter username:");
        if (fgets(username, sizeof(username), stdin) == NULL)
        {
            // fgets() returns NULL on error or EOF
            // however, even if user enters nothing, still returns \n
            // fprintf(stderr, "Error: Failed to read username.\n");
            continue; // Retry
        }
        size_t len = strlen(username);

        // Since fgets() truncates to fill the buffer, we need to check if the last character is a newline
        // if not we need to clear the input buffer to prevent issues with the next input;
        // also, if it trucated, we need to handle the case where username is too long
        if (len > 0 && username[len - 1] != '\n')
        {
            fprintf(stderr, "Sorry, limit your username to %d characters.\n", MAX_NAME_LEN);

            // Clear the input buffer
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;

            memset(username, '\0', sizeof(username)); // Clear the buffer
            continue;                                 // Reprompt the user
        }
        // Case to deal with empty username and remove newline character of username
        if (len >= 1 && username[len - 1] == '\n')
        {
            // Remove newline character
            username[len - 1] = '\0';
            len--;
            // fprintf(stderr, " name issue. \n"); // TODO: remove
        }
        if (len == 0)
        {
            continue; // Reprompt the user
        }
        else
        {
            break; // Valid username
        }
    }
    memset(outbuf, '\0', sizeof(outbuf));
    // Print greeting message
    printf("Hello, %s\n Let's try to connect to the server.\n", username);

    // Part 3
    // Step 1 - Create a TCP socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        fprintf(stderr, "Error: Failed to create socket. %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Step 2 - Connect to the server
    sa.sin_family = AF_INET;   // IPv4
    sa.sin_port = htons(port); // Port number in network byte order
    if (connect(client_socket, (struct sockaddr *)&sa, sizeof(sa)) == -1)
    {
        fprintf(stderr, "Error: Failed to connect to server. %s\n", strerror(errno));
        // close(client_socket); // Do I need to close the socket here? Or does exit() do it?
        exit(EXIT_FAILURE);
    }

    // Step 3 - Receive the welcome message from the server
    // use the providedd recv_with_length() to receive the message
    int bytes_received = recv_with_length(client_socket, inbuf, sizeof(inbuf));
    // if recv_with_length() returns 0, the server closed the connection
    if (bytes_received == 0)
    {
        fprintf(stderr, "Error: Server closed the connection.\n");
        exit(EXIT_FAILURE);
    }
    // if recv_with_length() returns -1, an error occurred
    else if (bytes_received == -1)
    {
        fprintf(stderr, "Error: Failed to receive message from server. %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Step 4 - Print the welcome message
    printf("\n%s\n\n", inbuf);

    // Step 5 - Send the username to the server
    // use the provided send_with_length() to send the message
    // username is already null-terminated, so we can use strlen() to get the length
    if (send_with_length(client_socket, username, strlen(username) + 1) == -1)
    {
        fprintf(stderr, "Error: Failed to send username to server. %s.\n", strerror(errno));
        memset(username, '\0', sizeof(username)); // Clear the buffer
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        fd_set read_fds;
        FD_ZERO(&read_fds);               // Clear the set
        FD_SET(STDIN_FILENO, &read_fds);  // Add stdin to the set
        FD_SET(client_socket, &read_fds); // Add the socket to the set

        int max_fd = client_socket; // Highest file descriptor to monitor

        // Wait for activity on stdin or the socket
        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1)
        {
            fprintf(stderr, "Error: select() failed. %s.\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        // Check if there's input from stdin
        if (FD_ISSET(STDIN_FILENO, &read_fds))
        {
            if (handle_stdin() == -1)
            {
                fprintf(stderr, "Error: Failed to handle stdin.\n");
                break;
            }
        }

        // Check if there's a message from the server
        if (FD_ISSET(client_socket, &read_fds))
        {
            if (handle_client_socket() == -1)
            {
                fprintf(stderr, "Error: Failed to handle client socket.\n");
                break;
            }
        }
    }

    close(client_socket); // Close the socket

    // if no error, exit with success
    return EXIT_SUCCESS;
}