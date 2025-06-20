#ifndef _UTIL_H_
#define _UTIL_H_

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_MSG_LEN 1024
#define MAX_NAME_LEN 20
// +4 = '[' before name, "]: " after name
#define BUFLEN MAX_MSG_LEN + MAX_NAME_LEN + 4

/* Functions that should be used in client and server. */
bool parse_int(const char *input, int *i, const char *usage);
int send_with_length(int socket, const char *msg, uint16_t len);
int recv_with_length(int socket, char *buf, size_t buf_size);

/**
 * Determines if the string input represent a valid integer.
 */
bool is_integer(const char *input) {
    int start = 0, len = strlen(input), i;

    if (len >= 1 && input[0] == '-') {
        if (len < 2) {
            return false;
        }
        start = 1;
    }
    for (i = start; i < len; i++) {
        if (!isdigit(input[i])) {
            return false;
        }
    }
    return len > 0;
}

/**
 * Attempts to convert the input string into the integer i.
 * Returns true if the conversion was successful, false otherwise.
 */
bool parse_int(const char *input, int *i, const char *usage) {
    long long long_long_i;

    if (strlen(input) == 0) {
        fprintf(stderr, "Error: Invalid input '' received for %s.\n", usage);
        return false;
    }
    if (is_integer(input) && sscanf(input, "%lld", &long_long_i) == 1) {
        *i = (int)long_long_i;
        if (long_long_i != *i) {
            fprintf(stderr, "Error: Integer overflow for %s.\n", usage);
            return false;
        }
    } else {
        fprintf(stderr, "Error: Invalid input '%s' received for %s.\n", input,
                usage);
        return false;
    }
    return true;
}

int send_with_length(int socket, const char *msg, uint16_t len) {
    uint16_t len_network = htons(len);
    if (send(socket, &len_network, sizeof(len_network), 0) !=
            sizeof(len_network) ||
        send(socket, msg, len, 0) != len) {
        return -1;
    }
    return 0;
}

int recv_with_length(int socket, char *buf, size_t buf_size) {
    uint16_t len;
    ssize_t bytes_recvd;

    if ((bytes_recvd = recv(socket, &len, sizeof(len), 0)) <= 0) {
        return bytes_recvd;
    }

    if (bytes_recvd != sizeof(len) || (len = ntohs(len)) > buf_size ||
        (bytes_recvd = recv(socket, buf, len, 0)) != len) {
        return -1;
    }
    return bytes_recvd;
}

#endif
