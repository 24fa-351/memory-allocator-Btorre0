#include <stdio.h>
#include <string.h>

#include "allocator.h"

void malloc_free_test() {
    printf("mallocing and freeing\n");

    allocator_init(1024);

    char *ptr = (char *)allocator_malloc(10);
    // strcpy(ptr, "Hello");

    if (ptr == NULL) {
        printf("Failed to allocate memory\n");
        allocator_cleanup();
        return;
    }

    strncpy(ptr, "World", 9);
    ptr[9] = '\0';

    printf("allocated and wrote - ptr: %s\n", ptr);

    allocator_free(ptr);
    printf("memory freed\n");
    allocator_cleanup();
}

void realloc_test() {
    printf("realloc test\n");

    allocator_init(1024);

    char *ptr = (char *)allocator_malloc(10);
    if (ptr == NULL) {
        printf("Failed to allocate memory\n");
        allocator_cleanup();
        return;
    }

    strncpy(ptr, "Hello", 9);
    ptr[9] = '\0';

    printf("allocated and wrote - ptr: %s\n", ptr);

    ptr = (char *)allocator_realloc(ptr, 20);
    if (ptr == NULL) {
        printf("Failed to reallocate memory\n");
        allocator_cleanup();
        return;
    }

    strncpy(ptr, "Hello, World!", 20);
    ptr[19] = '\0';

    printf("reallocated and wrote - ptr: %s\n", ptr);

    allocator_cleanup();
}

void merging_test() {
    allocator_init(1024);
    printf("Testing merging of free blocks...\n");

    void *ptr1 = allocator_malloc(100);
    void *ptr2 = allocator_malloc(100);

    allocator_free(ptr1);
    allocator_free(ptr2);

    void *ptr3 = allocator_malloc(200);
    if (ptr3) {
        printf("Merge successful.\n");
    } else {
        printf("Merge failed.\n");
    }

    allocator_cleanup();
}

#define NUM_THREADS 10

void* thread_worker(void *arg) {
    int *num = (int *)arg;
    printf("Thread %d\n", *num);
    free(num);
    return NULL;
}

void thread_test() {
    allocator_init(1024);
    pthread_t threads[NUM_THREADS];

    for (int ix = 0; ix < NUM_THREADS; ix++) {
        int *arg = (int *)allocator_malloc(sizeof(int));
        if (arg == NULL) {
            printf("Failed to allocate memory\n");
            exit(1);
        }
        *arg = ix;
        pthread_create(&threads[ix], NULL, thread_worker, arg);
    }

    for (int ix = 0; ix < NUM_THREADS; ix++) {
        pthread_join(threads[ix], NULL);
    }

    allocator_cleanup();
}

int main(int argc, char *argv[]) {
    malloc_free_test();
    realloc_test();
    merging_test();
    thread_test();

    return 0;
}