#include "allocator.h"

#include <stdio.h>
#include <string.h>

void malloc_free_test() {
    allocator_init(1024);
    char *ptr = (char *)allocator_malloc(10);
    strcpy(ptr, "Hello");
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

    allocator_cleanup();
}

int main() {
    malloc_free_test();
    realloc_test();
    return 0;
}