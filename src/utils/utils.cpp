#include "utils/utils.h"

using namespace SimpleOS;

void* SimpleOS::memset(void* ptr, int value, size_t num) {
	unsigned char* p = static_cast<unsigned char*>(ptr);
	for (size_t i = 0; i < num; i++) {
		p[i] = static_cast<unsigned char>(value);
	}
	return ptr;
}