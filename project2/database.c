#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "grades.h"

// Load grade entries from database file into linked list
bool load_database(const char *filename, GradeList *list) {
    if (!filename || !list) {
        return false;
    }
    
    // Open the database file for reading
    FILE *file = fopen(filename, "r");
    if (!file) {
        return false;
    }
    
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    
    // Read each line from the file
    while ((read = getline(&line, &len, file)) != -1) {
        // Remove ALL trailing whitespace including \r, \n, spaces, tabs
        // To possibly handle for both Windows (\r\n) and Linux (\n) line endings
        while (read > 0 && (line[read - 1] == '\n' || 
                            line[read - 1] == '\r' || 
                            line[read - 1] == ' ' || 
                            line[read - 1] == '\t')) {
            line[read - 1] = '\0';
            read--;
        }
        
        // Skip empty lines
        if (read == 0 || line[0] == '\0') {
            continue;
        }
        
        // Parse the line: STUDENT_ID:ASSIGNMENT_NAME:GRADE
        char student_id[12] = {0};
        char assignment[22] = {0};
        char grade_str[12] = {0};
        
        // Find first colon
        char *first_colon = strchr(line, ':');
        if (!first_colon) {
            continue;
        }
        
        // Find second colon
        char *second_colon = strchr(first_colon + 1, ':');
        if (!second_colon) {
            continue;
        }
        
        // Extract student ID (before first colon)
        int id_len = first_colon - line;
        if (id_len > 10 || id_len <= 0) {
            continue;
        }
        strncpy(student_id, line, id_len);
        student_id[id_len] = '\0';
        
        // Extract assignment name (between colons)
        int name_len = second_colon - first_colon - 1;
        if (name_len > 20 || name_len <= 0) {
            continue;
        }
        strncpy(assignment, first_colon + 1, name_len);
        assignment[name_len] = '\0';
        
        // Extract grade (after second colon)
        strncpy(grade_str, second_colon + 1, sizeof(grade_str) - 1);
        grade_str[sizeof(grade_str) - 1] = '\0';
        
        // Remove any trailing whitespace from grade string
        // (in case there are spaces or \r characters)
        size_t grade_len = strlen(grade_str);
        while (grade_len > 0 && (grade_str[grade_len - 1] == ' ' || 
                                  grade_str[grade_len - 1] == '\t' ||
                                  grade_str[grade_len - 1] == '\n' ||
                                  grade_str[grade_len - 1] == '\r')) {
            grade_str[grade_len - 1] = '\0';
            grade_len--;
        }
        
        // Validate and convert grade
        unsigned short grade;
        if (!is_valid_student_id(student_id) ||
            !is_valid_assignment_name(assignment) ||
            !is_valid_grade(grade_str, &grade)) {
            continue;  // Skip invalid entries
        }
        
        // Add the entry to the list
        add_entry(list, student_id, assignment, grade);
    }
    
    // Clean up
    free(line);
    fclose(file);
    return true;
}

// Save grade entries from linked list back to database file
bool save_database(const char *filename, GradeList *list) {
    if (!filename || !list) {
        return false;
    }
    
    // Create a temporary file for safe writing
    char temp_filename[] = "tmpXXXXXX";
    int temp_fd = mkstemp(temp_filename);
    if (temp_fd == -1) {
        fprintf(stderr, "Error: Failed to create temporary file\n");
        return false;
    }
    
    // Convert file descriptor to FILE pointer
    FILE *temp_file = fdopen(temp_fd, "w");
    if (!temp_file) {
        close(temp_fd);
        unlink(temp_filename);  // Delete temp file
        fprintf(stderr, "Error: Failed to open temporary file\n");
        return false;
    }
    
    // Write each entry to the temporary file
    Node *current = list->head;
    int entry_count = 0;
    while (current != NULL) {
        // Verify the node has valid data
        if (current->entry.studentId[0] == '\0') {
            fprintf(stderr, "Error: Empty student ID at entry %d\n", entry_count);
            fclose(temp_file);
            unlink(temp_filename);
            return false;
        }
        
        if (current->entry.assignmentName[0] == '\0') {
            fprintf(stderr, "Error: Empty assignment name at entry %d\n", entry_count);
            fclose(temp_file);
            unlink(temp_filename);
            return false;
        }
        
        // Write the entry
        int result = fprintf(temp_file, "%s:%s:%hu\n",
                current->entry.studentId,
                current->entry.assignmentName,
                current->entry.grade);
        
        if (result < 0) {
            fprintf(stderr, "Error: Failed to write entry %d\n", entry_count);
            fclose(temp_file);
            unlink(temp_filename);
            return false;
        }
        
        entry_count++;
        current = current->next;
    }
    
    // Close the temporary file
    if (fclose(temp_file) != 0) {
        fprintf(stderr, "Error: Failed to close temporary file\n");
        unlink(temp_filename);
        return false;
    }
    
    // Replace original file with temporary file
    if (rename(temp_filename, filename) != 0) {
        // Rename failed - delete temp file and report error
        fprintf(stderr, "Error: Failed to rename temporary file\n");
        unlink(temp_filename);
        return false;
    }
    
    return true;
}