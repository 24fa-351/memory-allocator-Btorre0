#include "allocator.h"

static void *base = NULL;
static mem_block_t *free_list = NULL;
static pthread_mutex_t allocator_mutex = PTHREAD_MUTEX_INITIALIZER;

void allocator_init(size_t size) {
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);
    base = sbrk(size);
    pthread_mutex_destroy(&lock);
    if (base == (void *)-1) {
        fprintf(stderr, "Failed to allocate memory\n");
        pthread_mutex_unlock(&allocator_mutex);
        exit(1);
    }
    // free block
    mem_block_t *block = (mem_block_t *)base;
    block->size = size - sizeof(mem_block_t);
    block->free = 1;
    block->next = NULL;
    free_list = block;

    pthread_mutex_unlock(&allocator_mutex);
}

void *allocator_malloc(size_t size) {
    if (size <= 0) {
        return NULL;
    }
    pthread_mutex_lock(&allocator_mutex);
    mem_block_t *block = free;

    while (block) {
        if (block->free && block->size >= size) {
            block->free = 0;
            return (void *)(block + 1);
        }
        block = block->next;
    }
    return NULL;
}

void allocator_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    mem_block_t *block = (mem_block_t *)ptr - 1;
    block->free = 1;

    mem_block_t *current = free_list;
    while (current->next) {
        if (current->free && current->next->free) {
            current->size += current->next->size + sizeof(mem_block_t);
            current->next = current->next->next;
        }
        current = current->next;
    }
    pthread_mutex_unlock(&allocator_mutex);
}

void *allocator_realloc(void *ptr, size_t size) {
    if (ptr == NULL) {
        return allocator_malloc(size);
    }
    mem_block_t *block = (mem_block_t *)ptr - 1;
    if (block->size >= size) {
        return ptr;
    }
    void *new_ptr = allocator_malloc(size);
    if (new_ptr == NULL) {
        return NULL;
    }
    memcpy(new_ptr, ptr, block->size);
    allocator_free(ptr);
    return new_ptr;
}

void allocator_cleanup() {
    brk(base);
    pthread_mutex_destroy(&allocator_mutex);
    free_list = NULL;
    base = NULL;
    pthread_mutex_unlock(&allocator_mutex);
}
