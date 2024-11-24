#include "allocator.h"

void* heap_init(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }
    return ptr;
}

void* free_heap(void *ptr) {
    free(ptr);
    return NULL;
}

void* malloc_heap(size_t size){
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }
    return ptr;
}

void* realloc_heap(void *ptr, size_t size) {
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL) {
        fprintf(stderr, "Failed to reallocate memory\n");
        exit(1);
    }
    return new_ptr;
}


void heap_cleanup() {
    return;
}
