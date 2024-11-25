#include "allocator.h"

void allocator_init(size_t size) {
    mem_block_t *block = (mem_block_t *)malloc(sizeof(mem_block_t));
    block->size = size;
    block->free = 1;
    block->next = NULL;
}

void *allocator_malloc(size_t size) {
    mem_block_t *block = (mem_block_t *)malloc(sizeof(mem_block_t));
    block->size = size;
    block->free = 0;
    block->next = NULL;
    return block;
}

void allocator_free(void *ptr) {
    mem_block_t *block = (mem_block_t *)ptr;
    block->free = 1;
}

void *allocator_realloc(void *ptr, size_t size) {
    mem_block_t *block = (mem_block_t *)ptr;
    block->size = size;
    return block;
}

void allocator_cleanup() {
}
