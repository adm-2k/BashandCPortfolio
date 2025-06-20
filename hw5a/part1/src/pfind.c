#define _POSIX_C_SOURCE 200809L // For PATH_MAX, getopt
#include <unistd.h>             // Getopt and optarg
#include <stdio.h>              // printf, fprintf
#include <stdlib.h>             // EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>
#include <regex.h>
#include <dirent.h>   // For opendir and closedir
#include <sys/stat.h> // For lstat()
#include <limits.h>   // For PATH_MAX

// Standard usage message
void print_usage(void)
{
    printf("Usage: ./pfind -d <directory> -p <permissions string> [-h]\n");
}

void recurse_directory(const char *dir_path, const char *perm_string)
{
    DIR *dir = opendir(dir_path);
    if (dir == NULL)
    {
        fprintf(stderr, "Error: Cannot open directory '%s'.\n", dir_path);
        return;
    }

    struct dirent *entry;          // Directory entry structure
    struct stat file_stat;         // File status structure
    char full_path_name[PATH_MAX]; // Buffer to hold the full path of the file

    while ((entry = readdir(dir)) != NULL)
    {
        // Skip "." and ".." to avoid infinite recursion
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // Construct the full path of the file/directory
        snprintf(full_path_name, PATH_MAX, "%s/%s", dir_path, entry->d_name);
        // full_path_name = malloc(strlen(dir_path) + strlen(entry->d_name) + 2);

        // Use lstat to get file information
        if (lstat(full_path_name, &file_stat) < 0)
        {
            fprintf(stderr, "Error: Cannot stat '%s'.\n", full_path_name);
            continue;
        }

        // Check if the file matches the permissions string (From Chapter 4.5)
        // We can't just get the permissions as a full string directly
        // so we need to check the file mode bits
        if (S_ISREG(file_stat.st_mode)) // Only checks regular files (not directories)
        {
            char file_perm[10];                                  // Buffer to hold the file permissions string
            snprintf(file_perm, 10, "%c%c%c%c%c%c%c%c%c",        // Format: drwxrwxrwx
                     (file_stat.st_mode & S_IRUSR) ? 'r' : '-',  // User read
                     (file_stat.st_mode & S_IWUSR) ? 'w' : '-',  // User write
                     (file_stat.st_mode & S_IXUSR) ? 'x' : '-',  // User execute
                     (file_stat.st_mode & S_IRGRP) ? 'r' : '-',  // Group read
                     (file_stat.st_mode & S_IWGRP) ? 'w' : '-',  // Group write
                     (file_stat.st_mode & S_IXGRP) ? 'x' : '-',  // Group execute
                     (file_stat.st_mode & S_IROTH) ? 'r' : '-',  // Other read
                     (file_stat.st_mode & S_IWOTH) ? 'w' : '-',  // Other write
                     (file_stat.st_mode & S_IXOTH) ? 'x' : '-'); // Other execute

            if (strcmp(file_perm, perm_string) == 0) // Compare perm strings
            {
                printf("%s\n", full_path_name); // If successful, print the absolute path of the matching file
            }
        }

        // If the entry is a directory, recurse into it
        if (S_ISDIR(file_stat.st_mode))
        {
            recurse_directory(full_path_name, perm_string);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[])
{
    // Using null as default to check that the options are set by end of getopt
    int opt;                  // variable to store the option character
    char *directory = NULL;   // directory as char pointer (string)
    char *perm_string = NULL; // permissions string as char pointer (string)

    // Use getopt to process command-line options
    while ((opt = getopt(argc, argv, "d:p:h")) != -1)
    {
        switch (opt)
        {

        case 'h':
            // If -h is passed, display usage and exit successfully
            print_usage();
            return EXIT_SUCCESS;
        case 'd':
            // If -d is passed, set the directory variable to the argument
            directory = optarg;
            break;
        case 'p':
            perm_string = optarg;
            break;
        case '?':
            // For unknown options, print an error message and exit with failure
            fprintf(stderr, "Error: Unknown option '-%c' received.\n", optopt);
            return EXIT_FAILURE;
        default:
            print_usage();
            return EXIT_FAILURE;
        }
    }

    // Validate that the program included directory and permissions string
    if (directory == NULL)
    {
        fprintf(stderr, "Error: Required argument -d <directory> not found.\n");
        return EXIT_FAILURE;
    }
    if (perm_string == NULL)
    {
        fprintf(stderr, "Error: Required argument -p <permissions string> not found.\n");
        return EXIT_FAILURE;
    }

    // Both args should be set — might need further processing/validating?
    // Next steps: Validate permissions string ✅, check directory existence ✅, etc.

    // Validate the permissions string using regex (Citation: NLP, OpenGroup and GNU C)
    regex_t regex;
    int return_value;
    // Compile regex pattern:
    // ^(r|-)(w|-)(x|-)(r|-)(w|-)(x|-)(r|-)(w|-)(x|-)$ --- Could I rewrite this with modulo per 3 characters?
    // Beginning of string ^, 3 sets of (r|-)(w|-)(x|-), and end of string $
    // regcomp returns 0 on success, non-zero on failure
    return_value = regcomp(&regex, "^(r|-)(w|-)(x|-)(r|-)(w|-)(x|-)(r|-)(w|-)(x|-)$", REG_EXTENDED);
    if (return_value) // Because regcomp returns 0 on success, if(ret) to check for failure
    {
        fprintf(stderr, "Error: Could not compile regex.\n");
        return EXIT_FAILURE;
    }
    // Execute regex compare on the permissions string
    return_value = regexec(&regex, perm_string, 0, NULL, 0);
    // &regex is reference to the compiled regex, perm_string is comparing string,
    // first 0 is for number of matches, NULL is a pointer to store match info, and last 0 is for no execution options
    regfree(&regex); // Free the compiled regex
    if (return_value != 0)
    {
        fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", perm_string);
        return EXIT_FAILURE;
    }

    /// Check if the directory exists and is accessible
    DIR *dir = opendir(directory);
    if (dir == NULL)
    {
        fprintf(stderr, "Error: Cannot open directory '%s'.\n", directory);
        return EXIT_FAILURE;
    }

    // If dir != NULL, successfully opened the directory, ergo it exists and is accessible
    closedir(dir);

    // Now both arguments have now been validated.
    // Next steps: need to implement recursion and file permission matching.

    // Call the recursive function to search for files with the specified permissions
    recurse_directory(directory, perm_string);

    return EXIT_SUCCESS;
}
