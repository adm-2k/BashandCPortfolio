#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>    // For exit, malloc, free
#include <unistd.h>    // For getcwd
#include <string.h>    // For strerror
#include <errno.h>     // For errno
#include <limits.h>    // For PATH_MAX
#include <pwd.h>       // For getpwuid in get_home
#include <signal.h>    // For sigaction, sigemptyset, and signal handling
#include <stdbool.h>   // For bool type
#include <sys/wait.h>  // For waitpid
#include <sys/types.h> // For pid_t

#define BRIGHT_BLUE "\x1b[34;1m"
#define DEFAULT "\x1b[0m"

// This helps deal with fatal errors, without having to register exit
void fatal(const char *msg);
void builtin_exit(int argc, char **argv);
void builtin_cd(int argc, char **argv);
void clp_writer(void);
char *quoted_helper(const char *arg);

#endif
