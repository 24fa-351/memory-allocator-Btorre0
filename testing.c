#include "allocator.h"

#include <stdio.h>
#include <string.h>

void malloc_free_test() {
    allocator_t allocator;
    allocator.init = heap_init;
    allocator.free = free_heap;
    allocator.malloc = malloc_heap;
    allocator.realloc = realloc_heap;
    allocator.cleanup = heap_cleanup;

    heap_t heap;
    heap.size = 10;
    heap.ptr = allocator.init(heap.size);
    if (heap.ptr == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return;
    }

    char *str = "Hello, World!";
    strcpy(heap.ptr, str);
    printf("Heap: %s\n", (char*)heap.ptr);

    heap.ptr = allocator.free(heap.ptr);
    if (heap.ptr != NULL) {
        fprintf(stderr, "Failed to free memory\n");
        return;
    }
}

void realloc_test() {
    allocator_t allocator;
    allocator.init = heap_init;
    allocator.free = free_heap;
    allocator.malloc = malloc_heap;
    allocator.realloc = realloc_heap;
    allocator.cleanup = heap_cleanup;

    heap_t heap;
    heap.size = 10;
    heap.ptr = allocator.init(heap.size);
    if (heap.ptr == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return;
    }

    char *str = "Hello, World!";
    strcpy(heap.ptr, str);
    printf("Heap: %s\n", (char*)heap.ptr);

    heap.size = 20;
    heap.ptr = allocator.realloc(heap.ptr, heap.size);
    if (heap.ptr == NULL) {
        fprintf(stderr, "Failed to reallocate memory\n");
        return;
    }

    char *str2 = "Hello, World! Hello, World!";
    strcpy(heap.ptr, str2);
    printf("Heap: %s\n", (char*)heap.ptr);

    heap.ptr = allocator.free(heap.ptr);
    if (heap.ptr != NULL) {
        fprintf(stderr, "Failed to free memory\n");
        return;
    }
}

int main() {
    malloc_free_test();
    realloc_test();
    return 0;
}