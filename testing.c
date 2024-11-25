#include "allocator.h"

#include <stdio.h>
#include <string.h>

void malloc_free_test() {
    allocator_init(1024);
    char *ptr = (char *)allocator_malloc(10);
    // strcpy(ptr, "Hello");
    if (ptr == NULL) {
        printf("Failed to allocate memory\n");
        return;
    }

    strcpy(ptr, "World");
    printf("allocated and wrote - ptr: %s\n", ptr);

    allocator_free(ptr);
    printf("memory freed\n");

    allocator_cleanup();
}

void realloc_test() {
    printf("reallocating\n");

    allocator_init(1024);
    char *ptr = (char *)allocator_malloc(10);
    strcpy(ptr, "Hello");

    if (ptr == NULL) {
        printf("Failed to allocate memory\n");
        return;
    }

    strcpy(ptr, "World");
    printf("allocated and wrote - ptr: %s\n", ptr);

    ptr = (char *)allocator_realloc(ptr, 20);
    if (ptr == NULL) {
        printf("Failed to reallocate memory\n");
        return;
    }

    strcpy(ptr, "Hello World");
    printf("reallocated and wrote - ptr: %s\n", ptr);
    allocator_free(ptr);

    allocator_cleanup();
}

void merging_test() {
    allocator_init(1024);
    printf("Testing merging of free blocks...\n");

    void* ptr1 = allocator_malloc(100);
    void* ptr2 = allocator_malloc(100);

    allocator_free(ptr1);
    allocator_free(ptr2);

    void* ptr3 = allocator_malloc(200);
    if (ptr3) {
        printf("Merge successful.\n");
    } else {
        printf("Merge failed.\n");
    }

    allocator_cleanup();
}

// #define NUM_THREADS 10

// void* thread_worker(void *arg) {
//     int *num = (int *)arg;
//     printf("Thread %d\n", *num);
//     free(num);
//     return NULL;
// }

// void thread_test() {
//     allocator_init(1024);
//     pthread_t threads[NUM_THREADS];
    
//     for (int i = 0; i < NUM_THREADS; i++) {
//         int *arg = malloc(sizeof(int));
//         if (arg == NULL) {
//             printf("Failed to allocate memory\n");
//             exit(1);
//         }
//         *arg = i;
//         pthread_create(&threads[i], NULL, thread_worker, arg);
//     }

//     for (int i = 0; i < NUM_THREADS; i++) {
//         pthread_join(threads[i], NULL);
//     }

//     allocator_cleanup();
// }

int main(int argc, char* argv[]) {
        if (argc > 1 && strcmp(argv[1], "-t") == 0) {
            int num_tests = atoi(argv[2]);
            if (num_tests == 1) {
                malloc_free_test();
            } else if (num_tests == 2) {
                realloc_test();
            } else if (num_tests == 3) {
                merging_test();
            } else {
                printf("Invalid test number\n");
            }
        } else {    
            malloc_free_test();
            realloc_test();
            merging_test();
        }
    // thread_test();
    return 0;
}