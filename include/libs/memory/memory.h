#pragma once

#ifndef _MEMORY_
#define _MEMORY_

#include "utils/typedef.h"

//#define allocate_ctor(__class, ...) (new (malloc(sizeof(__class))) __class(__VA_ARGS__))

/*#define deallocate_dtor(__ptr, __class) ({ \
//    if (__ptr) { \
//        __ptr->~__class(); \
//        free(__ptr); \
//    } \
//})*/

void* malloc(size_t size);
void free(void* ptr);
void* calloc(size_t num, size_t size);
void* realloc(void* ptr, size_t new_size);

inline void* operator new(size_t, void* ptr) noexcept {
    return ptr;
}

inline void operator delete(void*, void*) noexcept {}

template <typename _Ty, typename... Args>
_Ty* allocate_ctor(Args... args) {
    void* ptr = malloc(sizeof(_Ty));
    if (!ptr) return nullptr;

    return new(ptr) _Ty(args...);
}

template <typename _Ty>
void deallocate_dtor(_Ty* ptr) {
    if (ptr) {
        ptr->~_Ty();
        free(ptr);
    }
}

//void* operator new(size_t size);
//void operator delete(void* ptr) noexcept;

#endif // _MEMORY_