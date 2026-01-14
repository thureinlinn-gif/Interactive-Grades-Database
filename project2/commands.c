#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grades.h"

// Print all grade entries in a formatted table
void cmd_print(GradeList *list) {
    // Print table header with proper column widths
    printf("%-10s | %-20s | %5s\n", "Student ID", "Assignment Name", "Grade");
    printf("-----------------------------------------\n");
    
    // Print each entry
    Node *current = list->head;
    while (current) {
        printf("%-10s | %-20s | %5hu\n", current->entry.studentId, current->entry.assignmentName, current->entry.grade);
        current = current->next;
    }
}

// Calculate and print statistics for a specific assignment
void cmd_stats(GradeList *list, const char *assignment) {
    if (!list || !assignment) {
        return;
    }
    
    // Find all grades for this assignment
    int count = 0;
    int sum = 0;
    int min = 101;  // Start higher than max possible grade
    int max = -1;   // Start lower than min possible grade
    
    Node *current = list->head;
    while (current) {
        // Check if this entry matches the assignment name
        if (strcmp(current->entry.assignmentName, assignment) == 0) {
            int grade = current->entry.grade;
            
            // Update statistics
            sum += grade;
            count++;
            
            if (grade < min) {
                min = grade;
            }

            if (grade > max) {
                max = grade;
            }
        }
        current = current->next;
    }
    
    // Check if any entries were found
    if (count == 0) {
        printf("Error: No grades found for assignment '%s'\n", assignment);
        return;
    }
    
    // Calculate mean
    double mean = (double)sum / count;
    
    // Print statistics
    printf("Grade statistics for %s\n", assignment);
    printf("Min: %d\n", min);
    printf("Max: %d\n", max);
    printf("Mean: %.2f\n", mean);
}

// Process the add command
void cmd_add(GradeList *list, const char *args) {
    if (!list || !args) {
        printf("Error: Invalid argument\n");
        return;
    }
    
    // Skip any leading whitespace in args
    while (*args == ' ' || *args == '\t') {
        args++;
    }
    
    // Check if args is empty after skipping whitespace
    if (*args == '\0') {
        printf("Error: Invalid argument\n");
        return;
    }
    
    // Parse: STUDENT_ID:ASSIGNMENT_NAME:GRADE
    char student_id[11];
    char assignment[21];
    char grade_str[10];
    
    // Find first colon
    const char *first_colon = strchr(args, ':');
    if (!first_colon) {
        printf("Error: Invalid argument\n");
        return;
    }
    
    // Find second colon
    const char *second_colon = strchr(first_colon + 1, ':');
    if (!second_colon) {
        printf("Error: Invalid argument\n");
        return;
    }
    
    // Extract student ID
    int id_len = first_colon - args;
    if (id_len > 10 || id_len == 0) {
        printf("Error: Invalid argument\n");
        return;
    }
    strncpy(student_id, args, id_len);
    student_id[id_len] = '\0';
    
    // Extract assignment name
    int name_len = second_colon - first_colon - 1;
    if (name_len > 20 || name_len == 0) {
        printf("Error: Invalid argument\n");
        return;
    }
    strncpy(assignment, first_colon + 1, name_len);
    assignment[name_len] = '\0';
    
    // Extract grade string
    strncpy(grade_str, second_colon + 1, sizeof(grade_str) - 1);
    grade_str[sizeof(grade_str) - 1] = '\0';
    
    // Validate inputs
    unsigned short grade;
    if (!is_valid_student_id(student_id) || !is_valid_assignment_name(assignment) || !is_valid_grade(grade_str, &grade)) {
        printf("Error: Invalid argument\n");
        return;
    }
    
    // Try to add the entry
    if (!add_entry(list, student_id, assignment, grade)) {
        printf("Error: Entry already exists\n");
    }
}

// Process the remove command
void cmd_remove(GradeList *list, const char *args) {
    if (!list || !args) {
        printf("Error: Invalid argument\n");
        return;
    }
    
    // Parse: STUDENT_ID:ASSIGNMENT_NAME
    char student_id[11];
    char assignment[21];
    
    // Find the colon
    const char *colon = strchr(args, ':');
    if (!colon) {
        printf("Error: Invalid argument\n");
        return;
    }
    
    // Extract student ID
    int id_len = colon - args;
    if (id_len > 10) {
        printf("Error: Invalid argument\n");
        return;
    }
    strncpy(student_id, args, id_len);
    student_id[id_len] = '\0';
    
    // Extract assignment name
    strcpy(assignment, colon + 1);
    
    // Validate inputs
    if (!is_valid_student_id(student_id) || !is_valid_assignment_name(assignment)) {
        printf("Error: Invalid argument\n");
        return;
    }
    
    // Try to remove the entry
    if (!remove_entry(list, student_id, assignment)) {
        printf("Error: Entry not found\n");
    }
}

// Main command processor - determines which command to execute
void process_command(char *line, GradeList *list) {
    if (!line || !list) {
        return;
    }
    
    // Skip leading whitespace
    while (*line == ' ' || *line == '\t') {
        line++;
    }
    
    // Check for empty line
    if (*line == '\0') {
        return;
    }
    
    // Check which command was entered
    if (strncmp(line, "print", 5) == 0) {
        // Print command - ignore any arguments
        cmd_print(list);
    }
    else if (strncmp(line, "add ", 4) == 0) {
        // Add command - parse arguments after "add "
        cmd_add(list, line + 4);
    }
    else if (strncmp(line, "remove ", 7) == 0) {
        // Remove command - parse arguments after "remove "
        cmd_remove(list, line + 7);
    }
    else if (strncmp(line, "stats ", 6) == 0) {
        // Stats command - parse assignment name after "stats "
        const char *assignment = line + 6;
        if (is_valid_assignment_name(assignment)) {
            cmd_stats(list, assignment);
        } else {
            printf("Error: Invalid argument\n");
        }
    }
    else {
        // Unknown command
        printf("Error: Unknown command\n");
    }
}