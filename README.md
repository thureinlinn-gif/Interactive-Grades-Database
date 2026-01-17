# 📊 Interactive Grades Database System

A robust command-line database management system written in C for managing student grades with persistent storage and interactive commands.

---

## 🎯 Project Overview

This project implements a complete database system that allows users to view, add, remove, and analyze student grade records through an interactive command-line interface. The system uses a custom linked list data structure for in-memory operations and implements safe file I/O with atomic writes to prevent data corruption.

**Course:** CS265 - Advanced Programming Tools and Techniques  
**Language:** C  
**Key Concepts:** Data Structures, File I/O, Memory Management, Command Processing

---

## ✨ Key Features

### Core Functionality
- **Interactive Command Interface** - Real-time command processing with immediate feedback
- **Persistent Data Storage** - Reliable file-based database with atomic write operations
- **Dynamic Memory Management** - Custom linked list implementation with proper memory cleanup
- **Input Validation** - Comprehensive validation for all user inputs and data formats
- **Statistical Analysis** - Real-time calculation of min, max, and mean grades per assignment
- **Cross-Platform Compatibility** - Handles both Windows (CRLF) and Linux (LF) line endings

### Data Integrity
- **Atomic File Operations** - Uses temporary files to prevent data loss during saves
- **Duplicate Prevention** - Ensures no duplicate student-assignment combinations
- **Memory Safety** - Zero memory leaks verified with Valgrind
- **Error Handling** - Comprehensive error checking with informative messages

---

## 🏗️ System Architecture

### Data Structure Design

```
┌─────────────────────────────────────────────────────────────────┐
│                         GradeList                               │
│  ┌──────────┐                                                   │
│  │  head ───┼─────────────────────────────┐                    │
│  │  tail ───┼───────────────────┐         │                    │
│  │  count=3 │                   │         │                    │
│  └──────────┘                   │         │                    │
│                                 │         │                    │
│         ┌───────────────────────┼─────────┼──────────────┐    │
│         ▼                       │         ▼              │    │
│    ┌─────────┐              ┌─────────┐          ┌─────────┐ │
│    │  Node 1 │              │  Node 2 │          │  Node 3 │ │
│    ├─────────┤              ├─────────┤          ├─────────┤ │
│    │ Entry:  │              │ Entry:  │          │ Entry:  │ │
│    │  ID     │──next───────▶│  ID     │──next──▶│  ID     │ │
│    │  Name   │              │  Name   │          │  Name   │ │
│    │  Grade  │              │  Grade  │          │  Grade  │ │
│    └─────────┘              └─────────┘          └─────────┘ │
│                                                        │       │
│                                                     next=NULL  │
└─────────────────────────────────────────────────────────────────┘
```

### Module Organization

```
┌──────────────────────────────────────────────────────────────────┐
│                        grades (main)                             │
│  • Command-line argument parsing                                 │
│  • File permission validation                                    │
│  • Main command loop (reads stdin until EOF)                     │
└────────────┬─────────────────────────────────────┬──────────────┘
             │                                     │
             ▼                                     ▼
┌─────────────────────────┐         ┌──────────────────────────────┐
│   commands.c            │         │   database.c                 │
│  • Command parsing      │         │  • load_database()           │
│  • cmd_print()          │         │  • save_database()           │
│  • cmd_add()            │         │  • Atomic file operations    │
│  • cmd_remove()         │         │  • Cross-platform support    │
│  • cmd_stats()          │         └──────────────────────────────┘
└────────┬────────────────┘                        │
         │                                         │
         ▼                                         ▼
┌─────────────────────────┐         ┌──────────────────────────────┐
│   validation.c          │         │   list.c                     │
│  • is_valid_student_id()│         │  • create_list()             │
│  • is_valid_assignment()│         │  • add_entry()               │
│  • is_valid_grade()     │         │  • remove_entry()            │
└─────────────────────────┘         │  • free_list()               │
                                    └──────────────────────────────┘
```

---

## 🛠️ Technical Implementation

### File Structure

```
project/
├── grades.c           # Main program and entry point
├── grades.h           # Header with struct definitions and function declarations
├── list.c             # Linked list operations (add, remove, free)
├── validation.c       # Input validation functions
├── database.c         # File I/O operations (load, save)
├── commands.c         # Command processing and execution
├── makefile           # Build automation
└── sample.txt         # Database file (runtime)
```

### Database File Format

```
STUDENT_ID:ASSIGNMENT_NAME:GRADE

Example:
9991912292:HW 3:100
2145902184:HW 1:45
5352794201:Lab 7:42
```

**Constraints:**
- Student ID: Exactly 10 digits
- Assignment Name: 1-20 characters, no colons
- Grade: Integer 0-100

---

## 💻 Usage

### Compilation

```bash
make
```

**Output:**
```
gcc -Wall -Wextra -std=c99 -g -c grades.c -o grades.o
gcc -Wall -Wextra -std=c99 -g -c list.c -o list.o
gcc -Wall -Wextra -std=c99 -g -c validation.c -o validation.o
gcc -Wall -Wextra -std=c99 -g -c database.c -o database.o
gcc -Wall -Wextra -std=c99 -g -c commands.c -o commands.o
gcc -Wall -Wextra -std=c99 -g -o grades grades.o list.o validation.o database.o commands.o
```

### Execution

```bash
./grades sample.txt
```

---

## 📋 Available Commands

### 1. `print`
Displays all grade entries in a formatted table.

**Usage:**
```
print
```

**Example Output:**
```
Student ID | Assignment Name      | Grade
-----------------------------------------
9991912292 | HW 3                 |   100
2145902184 | HW 1                 |    45
5352794201 | Lab 1                |    65
```

---

### 2. `add STUDENT_ID:ASSIGNMENT_NAME:GRADE`
Adds a new grade entry to the database.

**Usage:**
```
add 1234567890:Quiz 1:85
```

**Success:** No output (silent success)  
**Failure:** `Error: Entry already exists` or `Error: Invalid argument`

---

### 3. `remove STUDENT_ID:ASSIGNMENT_NAME`
Removes an existing grade entry from the database.

**Usage:**
```
remove 1234567890:Quiz 1
```

**Success:** No output (silent success)  
**Failure:** `Error: Entry not found` or `Error: Invalid argument`

---

### 4. `stats ASSIGNMENT_NAME`
Displays statistical analysis for a specific assignment.

**Usage:**
```
stats Lab 7
```

**Example Output:**
```
Grade statistics for Lab 7
Min: 42
Max: 99
Mean: 72.33
```

---

### 5. Exit (EOF Signal)
Saves all changes and exits the program.

**Usage:**
- **Linux/Mac:** `Ctrl+D`
- **Windows:** `Ctrl+Z` then `Enter`

⚠️ **Important:** Always use EOF signal to exit. Using `Ctrl+C` will **discard all changes**!

---

## 📸 Demo Screenshots

### Initial Database View
```
Command: print
```
![Initial Database](screenshots/01_initial_print.png)
*Screenshot showing the initial state of the database with all entries*

---

### Adding a New Grade
```
Command: add 5352794201:HW 3:99
Command: print
```
![Adding Entry](screenshots/02_add_entry.png)
*Screenshot showing successful addition of a new grade entry*

---

### Statistical Analysis
```
Command: stats Lab 7
```
![Statistics](screenshots/03_stats.png)
*Screenshot showing min, max, and mean calculations for Lab 7*

---

### Removing an Entry
```
Command: remove 5352794201:My Cool Assignment 1
Command: print
```
![Removing Entry](screenshots/04_remove_entry.png)
*Screenshot showing successful removal of an entry*

---

### Error Handling
```
Command: remove abc
```
![Error Handling](screenshots/05_error_handling.png)
*Screenshot showing input validation and error messages*

---

### Saved Changes
```
$ cat sample.txt
```
![Saved Database](screenshots/06_saved_database.png)
*Screenshot showing the persisted database file after exiting with Ctrl+D*

---

## 🔧 Technical Highlights

### Memory Management
```c
// Custom linked list with proper cleanup
Node *current = list->head;
while (current) {
    Node *next = current->next;
    free(current);
    current = next;
}
```

**Verification:**
```bash
valgrind --leak-check=full ./grades sample.txt
```
**Result:** Zero memory leaks ✅

---

### Atomic File Operations
```c
// Safe write pattern: temp file → rename
1. Write to temporary file (tmpXXXXXX)
2. If successful, rename to target file
3. If failed, delete temp file
```

**Benefits:**
- Prevents data corruption if write fails
- Original file remains intact until successful completion
- POSIX atomic rename operation

---

### Input Validation
```c
// Multi-layer validation
1. Format validation (correct delimiters)
2. Length validation (10 digits, 1-20 chars)
3. Content validation (digits only, no special chars)
4. Range validation (0-100 for grades)
```

---

### Cross-Platform Line Ending Support
```c
// Handles both Windows (\r\n) and Linux (\n)
while (read > 0 && (line[read-1] == '\n' || 
                    line[read-1] == '\r')) {
    line[read-1] = '\0';
    read--;
}
```

---

## 🧪 Testing & Quality Assurance

### Memory Leak Testing
```bash
valgrind --leak-check=full --show-leak-kinds=all ./grades sample.txt
```

### Automated Testing
```bash
# Create test input file
cat > test_commands.txt << EOF
print
add 1234567890:Test:100
print
stats Test
remove 1234567890:Test
print
EOF

# Run automated test
./grades sample.txt < test_commands.txt
```

### Edge Cases Tested
- ✅ Empty database
- ✅ Duplicate entries
- ✅ Invalid input formats
- ✅ Maximum field lengths
- ✅ File permission errors
- ✅ Missing newline at EOF
- ✅ Windows/Linux line endings

---

## 🎓 Learning Outcomes

### Data Structures
- Implemented custom linked list with head/tail pointers
- Dynamic memory allocation and deallocation
- Efficient list traversal and manipulation

### Systems Programming
- File descriptor management
- POSIX system calls (mkstemp, rename)
- Signal handling (EOF detection)

### Software Engineering
- Modular design with clear separation of concerns
- Comprehensive error handling
- Memory safety and leak prevention
- Cross-platform compatibility

### C Programming
- Pointer manipulation
- String processing with bounds checking
- Structure packing and memory layout
- Function pointers and callbacks

---

## 🚀 Skills Demonstrated

| Skill | Implementation |
|-------|----------------|
| **C Programming** | Advanced pointer manipulation, string handling, memory management |
| **Data Structures** | Custom linked list implementation with O(n) operations |
| **File I/O** | Atomic writes, temporary files, cross-platform support |
| **Memory Management** | Zero-leak implementation verified with Valgrind |
| **Error Handling** | Comprehensive validation and informative error messages |
| **Testing** | Edge case testing, memory leak detection, automated tests |
| **Build Systems** | Makefile with multiple compilation units |
| **Code Quality** | Clean code, consistent style, extensive comments |

---

## 📊 Performance Characteristics

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Print all entries | O(n) | O(1) |
| Add entry | O(n) | O(1) |
| Remove entry | O(n) | O(1) |
| Calculate stats | O(n) | O(1) |
| Load database | O(n) | O(n) |
| Save database | O(n) | O(1) |

*where n = number of grade entries*

---

## 🔍 Code Quality Metrics

- **Lines of Code:** ~800 (excluding comments)
- **Memory Leaks:** 0 (Valgrind verified)
- **Compiler Warnings:** 0 (compiled with -Wall -Wextra)
- **Test Coverage:** All commands and edge cases tested
- **Documentation:** Comprehensive inline comments

---

## 📝 Build & Clean

### Build
```bash
make
```

### Clean
```bash
make clean
```

### Rebuild
```bash
make clean && make
```

---

## 🐛 Troubleshooting

### Issue: "Cannot read/write file"
**Solution:** Check file permissions
```bash
chmod 644 sample.txt
```

### Issue: "Segmentation fault"
**Solution:** Ensure file has proper line endings
```bash
dos2unix sample.txt  # Convert Windows → Linux
```

### Issue: Changes not saved
**Solution:** Exit with `Ctrl+D`, not `Ctrl+C`

### Issue: Command not found
**Solution:** Compile first with `make`

---

## 📚 Project Requirements Fulfilled

✅ Written entirely in C  
✅ Modular design with .c and .h files  
✅ Makefile for compilation  
✅ Command-line argument handling  
✅ File permission validation  
✅ Interactive stdin/stdout  
✅ Linked list data structure  
✅ All required commands (print, add, remove, stats)  
✅ Proper EOF handling with atomic saves  
✅ Zero memory leaks  
✅ Comprehensive error handling  

---

## 👨‍💻 Author

**[Thu Rein Linn]**  
Computer Science Student | Systems Programmer  
[LinkedIn]([your-linkedin-url](https://www.linkedin.com/in/thu-rein-linn-33950221b/)) | [GitHub]([your-github-url](https://github.com/thureinlinn-gif)) | [Email](trlinn0417@gmail.com)

---

## 📄 License

This project was created as part of CS265 coursework. All rights reserved.

---

## 🙏 Acknowledgments

- Course: CS265 - Advanced Programming Tools and Techniques
- Tools: GCC, Make, Valgrind, Git
- Platform: Linux (tested on Ubuntu/WSL)

---

**⭐ If you found this project interesting, please consider giving it a star!**
