#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

typedef struct {
    void* (*init)(size_t size);
    void* (*free)(void *ptr);
    void* (*malloc)(size_t size);
    void* (*realloc)(void *ptr, size_t size);
    void (*cleanup)();
} allocator_t;

typedef struct {
    size_t size;
    void *ptr;
} heap_t;

typedef struct {
    size_t size;
    void *ptr;
    void *next;
} heap_node_t;

void* free_heap(void *ptr);
void* malloc_heap(size_t size);
void* realloc_heap(void *ptr, size_t size);

void* heap_init(size_t size);
void heap_cleanup();

#endif // ALLOCATOR_H
