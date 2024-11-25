#pragma once

#ifndef __STRING_CLASS__
#define __STRING_CLASS__

#include "utils/typedef.h"
#include "libs/vector/vector.h"

namespace SimpleOS {

	class string {

	public:
		string();

		string(const char* str);

		string(const char c);

		string(const string& other);

		string(nullptr_t);

		string(size_t repeat, char c);

		string(const char* str, size_t length);

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

		string operator + (const string& other) const;

		string operator + (const char c) const;

		operator char* ();

		bool operator ==(const string& other) const;

		bool operator ==(const char* str) const;

		bool operator !=(const string& other) const;

		bool operator !=(const char* str) const;

		bool operator <(const char* other) const;

		bool operator >(const char* other) const;

		bool operator <(const string& other) const;

		bool operator >(const string& other) const;

		char& operator [](size_t index);

		const char& operator [](size_t index) const;

		bool empty() const;

		int to_int() const;

		static string to_string(int num);

		static int to_int(const string& str);
		
		size_t find(const string& sub, size_t start = 0) const;

		size_t find(char c, size_t start = 0) const;

		string substr(size_t start, size_t length) const;

		static string join(const string& delimiter, const vector<string>& parts);

		static constexpr const size_t npos = static_cast<size_t>(-1);

	private:
		char* str;
		size_t length;
	};
}

#endif // __STRING_CLASS__