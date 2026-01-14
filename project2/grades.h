#ifndef GRADES_H
#define GRADES_H

#include <stdbool.h>

// Structure to hold a single grade entry
struct GradeEntry {
    char studentId[11];        // 10-digit student ID + null terminator
    char assignmentName[21];   // Up to 20 chars + null terminator
    unsigned short grade;      // Grade value (0-100)
};

// Node in the linked list
typedef struct Node {
    struct GradeEntry entry;   // The grade entry data
    struct Node *next;         // Pointer to next node
} Node;

// Linked list structure
typedef struct {
    Node *head;                // Pointer to first node
    Node *tail;                // Pointer to last node
    int count;                 // Number of entries
} GradeList;

// Function declarations

// List management functions
GradeList* create_list(void);
void free_list(GradeList *list);
bool add_entry(GradeList *list, const char *student_id,  const char *assignment, unsigned short grade);
bool remove_entry(GradeList *list, const char *student_id, const char *assignment);

// Database I/O functions
bool load_database(const char *filename, GradeList *list);
bool save_database(const char *filename, GradeList *list);

// Command processing
void process_command(char *line, GradeList *list);
void cmd_print(GradeList *list);
void cmd_stats(GradeList *list, const char *assignment);

// Validation functions
bool is_valid_student_id(const char *id);
bool is_valid_assignment_name(const char *name);
bool is_valid_grade(const char *grade_str, unsigned short *grade);

#endif