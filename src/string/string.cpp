#include "libs/string/string.h"

void* memcpy(void* dest, const void* src, size_t n) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (unsigned char*)src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

size_t strlen(const char* str) {
    const char* s = str;
    while (*s) s++;
    return s - str;
}

char* strcpy(char* dest, const char* src) {
    char* start = dest;
    while ((*dest++ = *src++));
    return start;
}

char* strncpy(char* dest, const char* src, size_t n) {
    char* start = dest;
    while (n && *src) {
        *dest++ = *src++;
        n--;
    }
    while (n--) {
        *dest++ = '\0';
    }
    return start;
}

int strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}

char* strchr(const char* str, int c) {
    while (*str) {
        if (*str == (char)c) {
            return (char*)str;
        }
        str++;
    }
    return NULL;
}

int atoi(const char* str) {
    int result = 0;
    int sign = 1;
    while (*str == ' ') str++;
    if (*str == '-' || *str == '+') {
        sign = (*str == '-') ? -1 : 1;
        str++;
    }
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result * sign;
}

void itoa(int value, char* str, int base) {
    char* ptr = str;
    if (value < 0 && base == 10) {
        *ptr++ = '-';
        value = -value;
    }
    int num = value;
    do {
        int digit = num % base;
        *ptr++ = (digit < 10) ? '0' + digit : 'A' + (digit - 10);
        num /= base;
    } while (num > 0);
    *ptr = '\0';

    char* start = (*str == '-') ? str + 1 : str;
    char* end = ptr - 1;
    while (start < end) {
        char tmp = *start;
        *start++ = *end;
        *end-- = tmp;
    }
}

char* strcat(char* dest, const char* src) {
    char* start = dest;
    while (*dest) dest++;
    while ((*dest++ = *src++));
    return start;
}

char* strncat(char* dest, const char* src, size_t n) {
    char* start = dest;
    while (*dest) dest++;
    while (n-- && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    return start;
}

void to_upper(char* str) {
    while (*str) {
        if (*str >= 'a' && *str <= 'z') {
            *str = *str - ('a' - 'A');
        }
        str++;
    }
}

char to_upper(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - ('a' - 'A');
    }
    return c;
}

void to_lower(char* str) {
    while (*str) {
        if (*str >= 'A' && *str <= 'Z') {
            *str = *str + ('a' - 'A');
        }
        str++;
    }
}

char to_lower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}
