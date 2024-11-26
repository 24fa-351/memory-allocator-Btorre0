#include "allocator.h"

#include <pthread.h>
#include <sys/types.h>

static void *base = NULL;
static mem_block_t *free_list = NULL;
static pthread_mutex_t allocator_mutex = PTHREAD_MUTEX_INITIALIZER;

static mem_block_t *find_free_block(size_t size) {
    mem_block_t *block = free_list;
    while (block) {
        if (block->free && block->size >= size) {
            return block;
        }
        block = block->next;
    }
    return NULL;
}

void allocator_cleanup() {
    pthread_mutex_lock(&allocator_mutex);
    free_list = NULL;
    base = NULL;
    pthread_mutex_unlock(&allocator_mutex);
    pthread_mutex_destroy(&allocator_mutex);
}

void *get_me_blocks(ssize_t how_much) {
    if (how_much <= 0) {
        fprintf(stderr, "Invalid size\n", how_much);
        return NULL;
    }

    pthread_mutex_lock(&allocator_mutex);

    void *ptr = sbrk(0);

    if (ptr == (void *)-1) {
        perror("sbrk failed to get current break");
        pthread_mutex_unlock(&allocator_mutex);
        return NULL;
    }

    if (sbrk(how_much) == (void *)-1) {
        perror("sbrk failed to increase break");
        pthread_mutex_unlock(&allocator_mutex);
        return NULL;
    }

    pthread_mutex_unlock(&allocator_mutex);
    sbrk(how_much);
    return ptr;
}

static void split_block(mem_block_t *block, size_t size) {
    if (block->size > size + sizeof(mem_block_t)) {
        mem_block_t *new_block =
            (mem_block_t *)((char *)block + sizeof(mem_block_t) + size);
        new_block->size = block->size - size - sizeof(mem_block_t);
        new_block->free = 1;
        new_block->next = block->next;
        block->size = size;
        block->next = new_block;
    }
    block->free = 0;
}

void allocator_init(size_t size) {
    if (size < sizeof(mem_block_t)) {
        fprintf(stderr, "Size is too small\n");
        exit(1);
    }

    pthread_mutex_lock(&allocator_mutex);
    base = get_me_blocks(size);
    pthread_mutex_lock(&allocator_mutex);
    if (size < sizeof(mem_block_t)) {
        fprintf(stderr, "Initialization size too small\n");
        pthread_mutex_unlock(&allocator_mutex);
        exit(1);
    }
    base = sbrk(0);
    if (sbrk(size) == (void *)-1) {
        perror("sbrk failed");
        pthread_mutex_unlock(&allocator_mutex);
    }

    // mem_block_t *block = (mem_block_t *)base;
    free_list = (mem_block_t *)base;
    free_list->size = size - sizeof(mem_block_t);
    free_list->free = 1;
    free_list->next = NULL;

    pthread_mutex_unlock(&allocator_mutex);
}

void *allocator_malloc(size_t size) {
    if (size <= 0) {
        return NULL;
    }

    pthread_mutex_lock(&allocator_mutex);
    mem_block_t *block = find_free_block(size);
    if (block) {
        split_block(block, size);
        pthread_mutex_unlock(&allocator_mutex);
        memset((void *)(block + 1), 0, size);
        return (void *)(block + 1);
    }

    pthread_mutex_unlock(&allocator_mutex);
    return NULL;
}

void allocator_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    pthread_mutex_lock(&allocator_mutex);
    mem_block_t *block = (mem_block_t *)((char *)ptr - sizeof(mem_block_t));
    block->free = 1;

    mem_block_t *current = free_list;
    while (current) {
        if (current->free && current->next && current->next->free) {
            current->size += sizeof(mem_block_t) + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
    pthread_mutex_unlock(&allocator_mutex);
}

void *allocator_realloc(void *ptr, size_t size) {
    if (ptr == NULL) {
        return allocator_malloc(size);
    }

    pthread_mutex_lock(&allocator_mutex);
    mem_block_t *block = (mem_block_t *)((char *)ptr - sizeof(mem_block_t));
    if (block->size >= size) {
        pthread_mutex_unlock(&allocator_mutex);
        return ptr;
    }
    pthread_mutex_unlock(&allocator_mutex);
    void *new_ptr = allocator_malloc(size);
    if (new_ptr) {
        memcpy(new_ptr, ptr, block->size);
        allocator_free(ptr);
    }

    return new_ptr;
}

