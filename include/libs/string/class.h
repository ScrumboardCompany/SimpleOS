#pragma once

#ifndef __STRING_CLASS__
#define __STRING_CLASS__

#include "utils/typedef.h"

namespace SimpleOS {

	class string {

	public:
		string();

		string(const char* str);

		string(const char c);

		string(const string& other);

		string(nullptr_t);

		~string();

		const char* c_str() const;

		size_t size() const;

		void push(const char c);

		void push(const char c, size_t pos);

		void push(const string& other, size_t pos);

		void pop();

		void pop(size_t pos);

		void erase(size_t pos, size_t count = 1);

		void clear();

		string& operator = (const char* str);

		string& operator = (const string& other);

		string operator + (const string& other);

		string operator + (const char c);

		bool operator ==(const string& other) const;

		bool operator ==(const char* str) const;

		bool operator !=(const string& other) const;

		bool operator !=(const char* str) const;

		bool operator <(const char* other) const;

		bool operator >(const char* other) const;

		bool operator <(const string& other) const;

		bool operator >(const string& other) const;

		char& operator [](size_t index);

		bool empty() const;

	private:
		char* str;
		size_t length;
	};
}

#endif // __STRING_CLASS__