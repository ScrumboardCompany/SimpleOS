#include "utils/utils.h"

using namespace SimpleOS;

void* SimpleOS::memset(void* ptr, int value, size_t num) {
	unsigned char* p = static_cast<unsigned char*>(ptr);
	for (size_t i = 0; i < num; i++) {
		p[i] = static_cast<unsigned char>(value);
	}
	return ptr;
}

char* SimpleOS::itoa(int value, char* buffer, int base) {
    if (base < 2 || base > 36) {
        return nullptr;
    }

    int i = 0;
    bool isNegative = false;

    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return buffer;
    }

    if (value < 0 && base == 10) {
        isNegative = true;
        value = -value;
    }

    while (value != 0) {
        int remainder = value % base;
        buffer[i++] = (remainder > 9) ? (remainder - 10) + 'a' : remainder + '0';
        value = value / base;
    }

    if (isNegative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }

    return buffer;
}

size_t SimpleOS::strlen(const char* str) {
    size_t len = 0;
    while(str[len])
        ++len;
    return len;
}