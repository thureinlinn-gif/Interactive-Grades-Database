#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "grades.h"

// Check if a string is a valid 10-digit student ID
bool is_valid_student_id(const char *id) {
    if (!id) return false;
    
    // Must be exactly 10 characters
    if (strlen(id) != 10) return false;
    
    // All characters must be digits
    for (int i = 0; i < 10; i++) {
        if (!isdigit(id[i])) {
            return false;
        }
    }
    
    return true;
}

// Check if a string is a valid assignment name
bool is_valid_assignment_name(const char *name) {
    if (!name) return false;
    
    size_t len = strlen(name);
    
    // Must be between 1 and 20 characters
    if (len < 1 || len > 20) return false;
    
    // Cannot contain a colon
    if (strchr(name, ':') != NULL) return false;
    
    return true;
}

// Check if a string is a valid grade (0-100) and convert it
bool is_valid_grade(const char *grade_str, unsigned short *grade) {
    if (!grade_str || !grade) return false;
    
    // Check that all characters are digits
    size_t len = strlen(grade_str);
    if (len == 0) return false;
    
    for (size_t i = 0; i < len; i++) {
        if (!isdigit(grade_str[i])) {
            return false;
        }
    }
    
    // Convert to integer
    int value = atoi(grade_str);
    
    // Check range (0-100)
    if (value < 0 || value > 100) return false;
    
    // Store the value
    *grade = (unsigned short)value;
    return true;
}