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

    vector(const vector& other);

	void push(_Ty value);

	void pop();

    void pop(size_t index);

	_Ty& at(int pos);
    _Ty& operator[](int pos);
    const _Ty& operator[](int pos) const;
    _Ty& back();

	bool empty() const;

    void clear();

	bool valid_index(int pos) const;

	size_t size() const;

    void resize(size_t new_size);

    bool has(const _Ty& val) const;

    int find(const _Ty& value) const;

    _Ty* data();

    const _Ty* data() const;

    vector& operator=(const vector& other);

    ~vector();
};

template<typename _Ty>
inline vector<_Ty>::vector() : arr(nullptr), _size(0) {}

template<typename _Ty>
inline vector<_Ty>::vector(const vector<_Ty>& other) {
    _size = other._size;

    arr = (_Ty*)(malloc(_size * sizeof(_Ty)));

    for (size_t i = 0; i < _size; i++) {
        arr[i] = other.arr[i];
    }
}

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

    if (empty()) return;

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
inline void vector<_Ty>::pop(size_t index) {
    if (index >= _size || empty()) return;

    if (valid_index(index)) {
        _Ty* new_arr = (_Ty*)(malloc((_size - 1) * sizeof(_Ty)));

        for (size_t i = 0; i < index; i++) {
            new_arr[i] = arr[i];
        }

        for (size_t i = index + 1; i < _size; i++) {
            new_arr[i - 1] = arr[i];
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
    return arr[pos];
}

template<typename _Ty>
inline const _Ty& vector<_Ty>::operator[](int pos) const {
    return arr[pos];
}

template<typename _Ty>
inline _Ty& vector<_Ty>::back() {
    return arr[_size - 1];
}

template<typename _Ty>
inline bool vector<_Ty>::empty() const {
    return !_size;
}

template<typename _Ty>
inline void vector<_Ty>::clear() {
    if (empty()) return;
    _size = 0;
    if (arr) {
        free(arr);
        arr = nullptr;
    }
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
void vector<_Ty>::resize(size_t new_size) {
    if (new_size < _size) {
        _Ty* new_arr = (_Ty*)(malloc(new_size * sizeof(_Ty)));

        for (size_t i = 0; i < new_size; i++) {
            new_arr[i] = arr[i];
        }

        free(arr);
        arr = new_arr;
        _size = new_size;
    }
    else if (new_size > _size) {
        _Ty* new_arr = (_Ty*)(malloc(new_size * sizeof(_Ty)));

        for (size_t i = 0; i < _size; i++) {
            new_arr[i] = arr[i];
        }

        for (size_t i = _size; i < new_size; i++) {
            new_arr[i] = _Ty(); 
        }

        free(arr);
        arr = new_arr;
        _size = new_size;
    }
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
inline int vector<_Ty>::find(const _Ty& value) const {
    for (size_t i = 0; i < _size; i++) {

        if (arr[i] == value) {
            return i;
        }
    }
    return -1;
}

template<typename _Ty>
inline _Ty* vector<_Ty>::data() {
    return arr;
}

template<typename _Ty>
inline const _Ty* vector<_Ty>::data() const {
    return arr;
}

template<typename _Ty>
inline vector<_Ty>& vector<_Ty>::operator=(const vector<_Ty>& other) {
    if (this == &other) {
        return *this;
    }

    free(arr);

    _size = other._size;
    arr = (_Ty*)(malloc(_size * sizeof(_Ty)));

    for (size_t i = 0; i < _size; i++) {
        arr[i] = other.arr[i];
    }

    return *this;
}

template<typename _Ty>
inline vector<_Ty>::~vector() {
    free(arr);
}

#endif // _VECTOR_