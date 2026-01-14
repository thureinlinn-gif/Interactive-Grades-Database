#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grades.h"

// Create and initialize an empty linked list
GradeList* create_list(void) {
    // Allocate memory for the list structure
    GradeList *list = malloc(sizeof(GradeList));
    if (!list) {
        return NULL;
    }
    
    // Initialize empty list
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    
    return list;
}

// Free all memory used by the linked list
void free_list(GradeList *list) {
    if (!list) {
        return;
    }
    
    Node *current = list->head;
    // Traverse list and free each node
    while (current) {
        Node *next = current->next;  // Save next pointer before freeing
        free(current);                // Free current node
        current = next;               // Move to next node
    }
    
    // Free the list structure itself
    free(list);
}

// Add a new grade entry to the end of the list
bool add_entry(GradeList *list, const char *student_id, const char *assignment, unsigned short grade) {
    if (!list || !student_id || !assignment) {
        return false;
    }
    
    // Check if this student already has a grade for this assignment
    Node *current = list->head;
    while (current) {
        // Compare both student ID and assignment name
        if (strcmp(current->entry.studentId, student_id) == 0 && strcmp(current->entry.assignmentName, assignment) == 0) {
            // Duplicate found - cannot add
            return false;
        }
        current = current->next;
    }
    
    // Create a new node
    Node *new_node = malloc(sizeof(Node));
    if (!new_node) {
        return false;
    }
    
    // Initialize the new node completely
    memset(new_node, 0, sizeof(Node));
    
    // Copy data into the new node with bounds checking
    strncpy(new_node->entry.studentId, student_id, 10);
    new_node->entry.studentId[10] = '\0';  // Ensure null termination
    
    strncpy(new_node->entry.assignmentName, assignment, 20);
    new_node->entry.assignmentName[20] = '\0';  // Ensure null termination
    
    new_node->entry.grade = grade;
    new_node->next = NULL;
    
    // Add node to end of list
    if (list->tail) {
        // List is not empty - add after tail
        list->tail->next = new_node;
        list->tail = new_node;
    } else {
        // List is empty - this is the first node
        list->head = new_node;
        list->tail = new_node;
    }
    
    list->count++;
    return true;
}

// Remove a grade entry from the list
bool remove_entry(GradeList *list, const char *student_id, const char *assignment) {
    if (!list || !list->head) {
        return false;
    }
    
    Node *current = list->head;
    Node *prev = NULL;
    
    // Search for the entry to remove
    while (current) {
        // Check if this is the entry we're looking for
        if (strcmp(current->entry.studentId, student_id) == 0 &&
            strcmp(current->entry.assignmentName, assignment) == 0) {
            
            // Found it - now remove it from the list
            if (prev) {
                // Not the first node - link previous to next
                prev->next = current->next;
            } else {
                // This is the first node - update head
                list->head = current->next;
            }
            
            // If this was the last node, update tail
            if (current == list->tail) {
                list->tail = prev;
            }
            
            // Free the removed node
            free(current);
            list->count--;
            return true;
        }
        
        // Move to next node
        prev = current;
        current = current->next;
    }
    
    // Entry not found
    return false;
}