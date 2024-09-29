#include "utils/utils.h"
#include "utils/typedef.h"

using namespace SimpleOS;

void* SimpleOS::memset(void* ptr, int value, size_t num) {
    unsigned char* p = static_cast<unsigned char*>(ptr);
    for (size_t i = 0; i < num; i++) {
        p[i] = static_cast<unsigned char>(value);
    }
    return ptr;
}

char* SimpleOS::add_char(const char* str, char c) {
    size_t len = strlen(str);

    char* new_str = (char*)malloc(len + 2);
    if (!new_str) {
        return NULL;
    }

    strcpy(new_str, str);
    free((void*)str);

    new_str[len] = c;

    new_str[len + 1] = '\0';

    return new_str;
}

char* SimpleOS::pop_char(const char* str) {
    size_t len = strlen(str);

    if (len == 0) {
        char* empty_str = (char*)malloc(1);
        if (!empty_str) {
            return NULL;
        }
        empty_str[0] = '\0';
        return empty_str;
    }

    char* new_str = (char*)malloc(len);
    if (!new_str) {
        return NULL;
    }
     
    strncpy(new_str, str, len - 1);
    new_str[len - 1] = '\0';
    free((void*)str);

    return new_str;
}

char** SimpleOS::split(const char* str, char delimiter, int* size) {

    int count = 1;
    const char* tmp = str;
    while (*tmp) {
        if (*tmp == delimiter) {
            count++;
        }
        tmp++;
    }

    char** result = (char**)malloc((count + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    int index = 0;
    const char* start = str;
    const char* end = str;

    while (*end) {
        if (*end == delimiter) {
            int len = end - start;
            result[index] = (char*)malloc((len + 1) * sizeof(char));
            if (result[index] == NULL) {
                return NULL;
            }
            strncpy(result[index], start, len);
            result[index][len] = '\0';
            index++;
            start = end + 1;
        }
        end++;
    }

    int len = end - start;
    result[index] = (char*)malloc((len + 1) * sizeof(char));
    if (result[index] == NULL) {
        return NULL;
    }
    strncpy(result[index], start, len);
    result[index][len] = '\0';
    index++;

    result[index] = NULL;

    *size = count;

    return result;
}

size_t SimpleOS::get_size(char** array) {
    size_t size = 0;

    while (array[size]) {
        size++;
    }

    return size;
}