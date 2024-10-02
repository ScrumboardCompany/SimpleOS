#include "libs/memory/memory.h"

typedef struct Block {
    size_t size;
    struct Block* next;
    int free;
    int padding;
} Block;

#define HEAP_SIZE 0x100000
#define BLOCK_SIZE sizeof(Block)

static char heap[HEAP_SIZE];
static Block* free_list = NULL;

void init_memory() {
    free_list = (Block*)heap;
    free_list->size = HEAP_SIZE - BLOCK_SIZE;
    free_list->next = NULL;
    free_list->free = 1;
}

Block* find_free_block(size_t size) {
    Block* current = free_list;
    Block* best_fit = NULL;

    while (current != NULL) {
        if (current->free && current->size >= size) {
            if (best_fit == NULL || current->size < best_fit->size) {
                best_fit = current;
            }
        }
        current = current->next;
    }

    return best_fit;
}

void split_block(Block* block, size_t size) {
    if (block->size >= size + BLOCK_SIZE + 4) {
        Block* new_block = (Block*)((char*)block + BLOCK_SIZE + size);
        new_block->size = block->size - size - BLOCK_SIZE;
        new_block->free = 1;
        new_block->next = block->next;

        block->size = size;
        block->next = new_block;
    }
}

void* malloc(size_t size) {
    if (size <= 0) {
        return NULL;
    }

    if (free_list == NULL) {
        init_memory();
    }

    Block* block = find_free_block(size);
    if (block == NULL) {
        return NULL;
    }

    split_block(block, size);
    block->free = 0;
    return (void*)((char*)block + BLOCK_SIZE);
}

void free(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    Block* block = (Block*)((char*)ptr - BLOCK_SIZE);
    block->free = 1;

    Block* current = free_list;
    while (current != NULL) {
        if (current->free && current->next && current->next->free) {
            current->size += BLOCK_SIZE + current->next->size;
            current->next = current->next->next;
        }
        current = current->next;
    }
}

void* calloc(size_t num, size_t size) {
    size_t total_size = num * size;
    void* ptr = malloc(total_size);
    if (ptr != NULL) {
        for (size_t i = 0; i < total_size; i++) {
            ((char*)ptr)[i] = 0;
        }
    }
    return ptr;
}

void* realloc(void* ptr, size_t new_size) {
    if (ptr == NULL) {
        return malloc(new_size);
    }

    Block* block = (Block*)((char*)ptr - BLOCK_SIZE);
    if (block->size >= new_size) {
        return ptr;
    }

    void* new_ptr = malloc(new_size);
    if (new_ptr != NULL) {
        for (size_t i = 0; i < block->size; i++) {
            ((char*)new_ptr)[i] = ((char*)ptr)[i];
        }
        free(ptr);
    }
    return new_ptr;
}