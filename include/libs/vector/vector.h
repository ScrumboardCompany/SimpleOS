#pragma once

#ifndef _VECTOR_
#define _VECTOR_

#include "utils/typedef.h"
#include "libs/memory/memory.h"

template<typename _Ty>
class vector {
	_Ty* arr;
	size_t _size;

public:
	vector();

	void push(_Ty value);

	void pop();

	_Ty& at(int pos);
    _Ty& operator[](int pos);

	bool empty() const;

	bool valid_index(int pos) const;

	size_t size() const;

    bool has(const _Ty& val) const;

    ~vector();
};

template<typename _Ty>
inline vector<_Ty>::vector() : arr(nullptr), _size(0) {}

template<typename _Ty>
inline void vector<_Ty>::push(_Ty value) {
    _Ty* new_arr = (_Ty*)(malloc((_size + 1) * sizeof(_Ty)));

    for (size_t i = 0; i < _size; i++) {
        new_arr[i] = arr[i];
    }
    new_arr[_size] = value;

    free(arr);
    arr = new_arr;

    ++_size;
}

template<typename _Ty>
inline void vector<_Ty>::pop() {

    if (_size) {
        _Ty* new_arr = (_Ty*)(malloc((_size + 1) * sizeof(_Ty)));

        for (size_t i = 0; i < _size - 1; i++) {
            new_arr[i] = arr[i];
        }

        free(arr);
        arr = new_arr;

        --_size;
    }
}

template<typename _Ty>
inline _Ty& vector<_Ty>::at(int pos) {
    return arr[pos];
}

template<typename _Ty>
inline _Ty& vector<_Ty>::operator[](int pos) {
    return at(pos);
}

template<typename _Ty>
inline bool vector<_Ty>::empty() const {
    return !_size;
}

template<typename _Ty>
inline bool vector<_Ty>::valid_index(int pos) const {
    return pos >= 0 && pos < (int)_size;
}

template<typename _Ty>
inline size_t vector<_Ty>::size() const {
    return _size;
}

template<typename _Ty>
inline bool vector<_Ty>::has(const _Ty& val) const{
    for (size_t i = 0; i < _size; i++) {
        if (val == arr[i])
            return true;
    }
    return false;
}

template<typename _Ty>
inline vector<_Ty>::~vector() {
    free(arr);
}

#endif // _VECTOR_