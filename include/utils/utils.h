#pragma once

#ifndef _UTILS_
#define _UTILS_

#include "utils/typedef.h"

namespace SimpleOS {
	void* memset(void* ptr, int value, size_t num);

    char* itoa(int value, char* buffer, int base = 10);

    size_t strlen(const char* str);

    int atoi(const char* str);

    char* add_char(const char* str, char c, size_t* size);

    char* strncpy(char* dest, const char* src, size_t n);

    char** split(const char* str, char delimiter, int* size);

    size_t get_size(char** array);
}

#endif // _UTILS_