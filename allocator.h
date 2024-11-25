#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    size_t size;
    int free;
    struct mem_block *next;
} mem_block_t;

void allocator_init(size_t size);
void *allocator_malloc(size_t size);
void allocator_free(void *ptr);
void *allocator_realloc(void *ptr, size_t size);
void allocator_cleanup();

#endif  // ALLOCATOR_H
