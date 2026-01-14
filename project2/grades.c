#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include "grades.h"

// Check if a file exists and can be read/written
bool file_exists(char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

// Check if file has read and write permissions
bool can_read_write(char *filename) {
    return (access(filename, R_OK | W_OK) == 0);
}

int main(int argc, char *argv[]) {
    // Check that exactly one argument (database file path) was provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s DATABASE_FILE\n", argv[0]);
        return 1;
    }

    char *db_file = argv[1];

    // Verify the database file exists
    if (!file_exists(db_file)) {
        fprintf(stderr, "Error: File '%s' does not exist\n", db_file);
        return 1;
    }

    // Verify we can read and write to the file
    if (!can_read_write(db_file)) {
        fprintf(stderr, "Error: Cannot read/write file '%s'\n", db_file);
        return 1;
    }

    // Create an empty linked list to store grade entries
    GradeList *list = create_list();
    if (!list) {
        fprintf(stderr, "Error: Failed to create list\n");
        return 1;
    }

    // Load the database file into the linked list
    if (!load_database(db_file, list)) {
        fprintf(stderr, "Error: Failed to load database\n");
        free_list(list);
        return 1;
    }

    // Buffer to store each line of user input
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    // Main command loop: read commands until EOF (Ctrl+D)
    while ((read = getline(&line, &len, stdin)) != -1) {
        // Remove newline character at end of input
        if (read > 0 && line[read - 1] == '\n') {
            line[read - 1] = '\0';
            read--;
        }

        // Process the command entered by user
        process_command(line, list);
    }

    // Free the input buffer
    free(line);

    // Save the modified database back to file
    if (!save_database(db_file, list)) {
        fprintf(stderr, "Error: Failed to save database\n");
        free_list(list);
        return 1;
    }

    // Clean up: free all allocated memory
    free_list(list);
    return 0;
}