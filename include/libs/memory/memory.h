#pragma once

#ifndef _MEMORY_
#define _MEMORY_

#include "utils/typedef.h"

void* malloc(size_t size);
void free(void* ptr);
void* calloc(size_t num, size_t size);
void* realloc(void* ptr, size_t new_size);

#endif // _MEMORY_