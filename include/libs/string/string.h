#pragma once

#ifndef _STRING_
#define _STRING_

#include "utils/typedef.h"

void* memcpy(void* dest, const void* src, size_t n);
size_t strlen(const char* str);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
int strcmp(const char* str1, const char* str2);
char* strchr(const char* str, int c);
int atoi(const char* str);
void itoa(int value, char* str, int base);
char* strcat(char* dest, const char* src);
char* strncat(char* dest, const char* src, size_t n);

void to_upper(char* str);
char to_upper(char c);
void to_lower(char* str);
char to_lower(char c);

#endif // _STRING_