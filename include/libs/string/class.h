#pragma once

#ifndef __CLASS__
#define __CLASS__

#include "utils/typedef.h"

namespace SimpleOS {

	class string {

	public:
		string();

		string(const char* str);

		string(const string& other);

		string(nullptr_t);

		~string();

		const char* c_str() const;

		size_t size() const;

		void push(const char c);

		void pop();

		void clear();

		string& operator = (const char* str);

		string& operator = (const string& other);

		string operator + (const string& other);

		bool operator ==(const string& other) const;

		bool operator !=(const string& other) const;

		bool operator <(const char* other) const;

		bool operator >(const char* other) const;

		bool operator <(const string& other) const;

		bool operator >(const string& other) const;

		char& operator [](size_t index);

	private:
		char* str;
		size_t length;
	};
}

#endif // __CLASS__