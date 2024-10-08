#include "utils/utils.h"
#include "utils/typedef.h"
#include "libs/io/io.h"

using namespace SimpleOS;

time SimpleOS::get_time() {
    auto read_rtc = [](u8 reg) {
        outb(0x70, reg);
        return inb(0x71);
        };

    auto bcd_to_binary = [](unsigned char bcd) {
        return (bcd & 0x0F) + ((bcd / 16) * 10);
        };

    time current_time;

    current_time.second = bcd_to_binary(read_rtc(0x00));
    current_time.minute = bcd_to_binary(read_rtc(0x02));
    current_time.hour   = bcd_to_binary(read_rtc(0x04));
    current_time.day    = bcd_to_binary(read_rtc(0x07));
    current_time.month  = bcd_to_binary(read_rtc(0x08));
    current_time.year   = bcd_to_binary(read_rtc(0x09));

    return current_time;
}

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

size_t SimpleOS::abs(int x) {
    return x < 0 ? -x : x;
}