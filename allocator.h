#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
typedef struct mem_block{
    size_t size;
    int free;
    struct mem_block *next;
} mem_block_t;

static mem_block_t *find_free_block(size_t size);
static void split_block(mem_block_t *block, size_t size);

void allocator_init(size_t size);
void *allocator_malloc(size_t size);
void allocator_free(void *ptr);
void *allocator_realloc(void *ptr, size_t size);
void allocator_cleanup();

#endif  // ALLOCATOR_H
