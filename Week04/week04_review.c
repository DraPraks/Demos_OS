/*
 * Week04 Review Program for OS Midterms
 * Demonstrates key concepts: variables, pointers, structs, memory addresses
 * Copyright (C) 2023 - Based on BinKadal examples
 */

#include <stdio.h>
#include <stdlib.h>

// Global variables (stored in data segment)
int global_int = 42;
char global_char = 'G';

// Struct definition
typedef struct {
    char* name;
    int age;
    int semester;
    char* nim;
} Student;

// Function to demonstrate passing parameters
void pass_by_value(int x) {
    x = x + 10;
    printf("Inside pass_by_value: x = %d\n", x);
}

void pass_by_reference(int* x) {
    *x = *x + 10;
    printf("Inside pass_by_reference: *x = %d\n", *x);
}

// Function to print student struct
void print_student(Student* s) {
    printf("Name: %-10s NIM: %s Age: %d Semester: %d\n",
           s->name, s->nim, s->age, s->semester);
}

int main(void) {
    printf("=== WEEK04 REVIEW: C Programming Concepts ===\n\n");

    // 1. Global vs Local Variables and Addresses
    printf("1. GLOBAL VS LOCAL VARIABLES AND ADDRESSES\n");
    printf("Global int:  value=%d, address=%p\n", global_int, (void*)&global_int);
    printf("Global char: value='%c', address=%p\n", global_char, (void*)&global_char);

    int local_int = 24;
    char local_char = 'L';
    printf("Local int:   value=%d, address=%p\n", local_int, (void*)&local_int);
    printf("Local char:  value='%c', address=%p\n", local_char, (void*)&local_char);
    printf("\n");

    // 2. Pointers
    printf("2. POINTERS\n");
    int* ptr_int = &local_int;
    char* ptr_char = &local_char;
    printf("ptr_int points to: value=%d, address=%p, pointer address=%p\n",
           *ptr_int, (void*)ptr_int, (void*)&ptr_int);
    printf("ptr_char points to: value='%c', address=%p, pointer address=%p\n",
           *ptr_char, (void*)ptr_char, (void*)&ptr_char);
    printf("\n");

    // 3. Pointers of Pointers
    printf("3. POINTERS OF POINTERS\n");
    int** ptr_ptr_int = &ptr_int;
    char** ptr_ptr_char = &ptr_char;
    printf("ptr_ptr_int: **ptr=%d, *ptr=%p, ptr=%p, address=%p\n",
           **ptr_ptr_int, (void*)*ptr_ptr_int, (void*)ptr_ptr_int, (void*)&ptr_ptr_int);
    printf("ptr_ptr_char: **ptr='%c', *ptr=%p, ptr=%p, address=%p\n",
           **ptr_ptr_char, (void*)*ptr_ptr_char, (void*)ptr_ptr_char, (void*)&ptr_ptr_char);
    printf("\n");

    // 4. Pointers of Pointers of Pointers
    printf("4. POINTERS OF POINTERS OF POINTERS\n");
    int*** ptr_ptr_ptr_int = &ptr_ptr_int;
    printf("ptr_ptr_ptr_int: ***ptr=%d, **ptr=%p, *ptr=%p, ptr=%p, address=%p\n",
           ***ptr_ptr_ptr_int, (void*)*ptr_ptr_ptr_int, (void*)ptr_ptr_ptr_int, (void*)&ptr_ptr_ptr_int);
    printf("\n");

    // 5. Char Pointer vs Int Pointer
    printf("5. CHAR POINTER VS INT POINTER\n");
    char* chr_ptr = &global_char;
    int* int_ptr = &global_int;
    printf("char pointer: size=%zu bytes, points to 1 byte\n", sizeof(chr_ptr));
    printf("int pointer:  size=%zu bytes, points to %zu bytes\n", sizeof(int_ptr), sizeof(int));
    printf("char value: %c (hex: %#x)\n", *chr_ptr, *chr_ptr);
    printf("int value:  %d (hex: %#x)\n", *int_ptr, *int_ptr);
    printf("\n");

    // 6. Passing Parameters
    printf("6. PASSING PARAMETERS\n");
    int test_val = 5;
    printf("Before pass_by_value: test_val = %d\n", test_val);
    pass_by_value(test_val);
    printf("After pass_by_value: test_val = %d\n", test_val);

    printf("Before pass_by_reference: test_val = %d\n", test_val);
    pass_by_reference(&test_val);
    printf("After pass_by_reference: test_val = %d\n", test_val);
    printf("\n");

    // 7. Structs
    printf("7. STRUCTS\n");
    Student local_student = {"Alice", 20, 4, "1234567890"};
    Student* ptr_student = &local_student;

    printf("Direct access: ");
    print_student(&local_student);

    printf("Pointer access: ");
    print_student(ptr_student);

    // Modify via pointer
    ptr_student->age = 21;
    printf("After modification: ");
    print_student(ptr_student);
    printf("\n");

    // 8. Arrays and Pointers (bonus)
    printf("8. ARRAYS AND POINTERS\n");
    int arr[5] = {10, 20, 30, 40, 50};
    int* arr_ptr = arr;
    printf("Array elements via pointer:\n");
    for(int i = 0; i < 5; i++) {
        printf("arr[%d] = %d, via *(arr_ptr + %d) = %d, address: %p\n",
               i, arr[i], i, *(arr_ptr + i), (void*)(arr_ptr + i));
    }
    printf("\n");

    printf("=== END OF WEEK04 REVIEW ===\n");
    return 0;
}
