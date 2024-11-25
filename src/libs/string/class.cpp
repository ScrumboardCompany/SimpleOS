#include "libs/string/class.h"
#include "libs/string/string.h"
#include "libs/memory/memory.h"

using namespace SimpleOS;

string::string() {
	str = nullptr;
	length = 0;
};

string::string(const char* str) {
	length = strlen(str);
	this->str = (char*)malloc(length + 1);

	for (size_t i = 0; i < length; i++)
	{
		this->str[i] = str[i];
	}

	this->str[length] = '\0';
};

string::string(const char c) {
	length = 1;
	this->str = (char*)malloc(2);

	this->str[0] = c;

	this->str[1] = '\0';
}

string::string(const string& other) {
	if (other.str == nullptr) {
		str = nullptr;
		length = 0;
		return;
	}
	length = other.length;
	str = (char*)malloc(length + 1);

	for (size_t i = 0; i < length; i++) {
		str[i] = other.str[i];
	}
	str[length] = '\0';
}

string::string(nullptr_t) : string() {}

string::string(size_t repeat, char c) {
	length = repeat;
	str = (char*)malloc(length + 1);

	for (size_t i = 0; i < repeat; i++) {
		str[i] = c;
	}

	str[length] = '\0';
}

string::string(const char* str, size_t length) {
	this->length = length;
	this->str = (char*)malloc(length + 1);

	for (size_t i = 0; i < length; i++) {
		this->str[i] = str[i];
	}

	this->str[length] = '\0';
}

string::~string() {
	if(str) 
		free(str);
}

const char* string::c_str() const {
	return this->str;
}

size_t string::size() const {
	return this->length;
}

void string::push(const char c) {
	string newStr;

	newStr.str = (char*)malloc(length + 2);
	newStr.length = length + 1;

	for (size_t i = 0; i < length; i++) {
		newStr.str[i] = str[i];
	}

	newStr[newStr.length - 1] = c;

	newStr.str[newStr.length] = '\0';

	this->operator=(newStr);
}

void string::push(const char c, size_t pos) {
	if (pos > length) pos = length;

	string newStr;

	newStr.str = (char*)malloc(length + 2); 
	newStr.length = length + 1;

	for (size_t i = 0; i < pos; i++) {
		newStr.str[i] = str[i];
	}

	newStr.str[pos] = c;

	for (size_t i = pos; i < length; i++) {
		newStr.str[i + 1] = str[i];
	}

	newStr.str[newStr.length] = '\0';

	this->operator=(newStr);
}

void string::push(const string& other, size_t pos) {
	if (pos > length) pos = length;
	
	for (size_t i = 0; i < other.length; i++) {
		push(other.str[i], pos++);
	}
}

void string::pop() {
	if (empty()) return;

	string newStr;

	newStr.str = (char*)malloc(length);
	newStr.length = length - 1;

	for (size_t i = 0; i < newStr.length; i++) {
		newStr.str[i] = str[i];
	}

	newStr.str[newStr.length] = '\0';

	this->operator=(newStr);
}

void string::pop(size_t pos) {
	if (pos >= length || empty()) return;

	string newStr;

	newStr.str = (char*)malloc(length); 
	newStr.length = length - 1;

	for (size_t i = 0; i < pos; i++) {
		newStr.str[i] = str[i];
	}

	for (size_t i = pos + 1; i < length; i++) {
		newStr.str[i - 1] = str[i];
	}

	newStr.str[newStr.length] = '\0';

	this->operator=(newStr);
}

void string::erase(size_t pos, size_t count) {
	if (pos >= length || count == 0 || empty()) return;
	if (pos + count > length) count = length - pos;

	string newStr;
	newStr.length = length - count;
	newStr.str = (char*)malloc(newStr.length + 1);

	for (size_t i = 0; i < pos; i++) {
		newStr.str[i] = str[i];
	}

	for (size_t i = pos + count; i < length; i++) {
		newStr.str[i - count] = str[i];
	}

	newStr.str[newStr.length] = '\0';
	this->operator=(newStr);
}

void string::clear() {
	if (str) {
		free(str);
		str = nullptr;
	}
	length = 0;
}

string& string::operator = (const char* str) {
	if (str) {
		free(this->str);
	}
	length = strlen(str);
	this->str = (char*)malloc(length + 1);

	for (size_t i = 0; i < length; i++)
	{
		this->str[i] = str[i];
	}

	this->str[length] = '\0';

	return *this;
};

string& string::operator = (const string& other) {
	if (this == &other)
		return *this;
	if (str)
		free(str);
	length = strlen(other.str);
	this->str = (char*)malloc(length + 1);

	for (size_t i = 0; i < length; i++)
	{
		this->str[i] = other.str[i];
	}

	this->str[length] = '\0';

	return *this;
};

string string::operator + (const string& other) const {
	string newStr;

	size_t thisLen = length;
	size_t otherLen = strlen(other.str);

	newStr.length = thisLen + otherLen;

	newStr.str = (char*)malloc(newStr.length + 1);

	for (size_t i = 0; i < thisLen; i++) {
		newStr.str[i] = str[i];
	}

	for (size_t i = 0; i < otherLen; i++) {
		newStr.str[i + thisLen] = other.str[i];
	}

	newStr.str[newStr.length] = '\0';

	return newStr;
};

string string::operator+(const char c) const {
	string newStr;

	size_t thisLen = length;

	newStr.length = thisLen + 1;

	newStr.str = (char*)malloc(newStr.length + 1);

	for (size_t i = 0; i < thisLen; i++) {
		newStr.str[i] = str[i];
	}

	newStr.str[thisLen] = c;

	newStr.str[thisLen + 1] = '\0';

	return newStr;
}

string::operator char* () {
	return str;
}

bool string::operator ==(const string& other) const {
	return strcmp(str, other.str) == 0;
};

bool string::operator ==(const char* str) const {
	return strcmp(this->str, str) == 0;
};

bool string::operator !=(const string& other) const {
	return strcmp(str, other.str) != 0;
};

bool string::operator !=(const char* str) const {
	return strcmp(this->str, str) != 0;
};

bool string::operator <(const char* other) const {
	return strcmp(str, other) < 0;
}

bool string::operator >(const char* other) const {
	return strcmp(str, other) > 0;
}

bool string::operator <(const string& other) const {
	return strcmp(str, other.str) < 0;
}

bool string::operator >(const string& other) const {
	return strcmp(str, other.str) > 0;
}

char& string::operator [](size_t index) {
	return str[index];
};

const char& string::operator [](size_t index) const {
	return str[index];
}

bool string::empty() const {
	return str == nullptr || str[0] == '\0';
}

int string::to_int() const {
	return to_int(*this);
}

string string::to_string(int num) {
	string result;
	itoa(num, (char*)result, 10);

	return result;
}

int string::to_int(const string& str) {
	return atoi(str.c_str());
}

size_t SimpleOS::string::find(const string& sub, size_t start) const {
	if (start >= length || sub.length == 0) return npos;

	for (size_t i = start; i <= length - sub.length; ++i) {
		bool match = true;
		for (size_t j = 0; j < sub.length; ++j) {
			if (str[i + j] != sub[j]) {
				match = false;
				break;
			}
		}
		if (match) return i;
	}
	return size_t(-1);
}

size_t SimpleOS::string::find(char c, size_t start) const {
	if (start >= length) return npos;

	for (size_t i = start; i < length; ++i) {
		if (str[i] == c) return i;
	}
	return size_t(-1);
}

SimpleOS::string SimpleOS::string::substr(size_t start, size_t len) const {
	if (start >= length) return string();
	if (start + len > length) len = length - start;

	string result(len, '\0');
	for (size_t i = 0; i < len; ++i) {
		result[i] = str[start + i];
	}
	return result;
}

SimpleOS::string SimpleOS::string::join(const string& delimiter, const vector<string>& parts) {
	if (parts.empty()) return string();

	size_t total_length = 0;
	for (size_t i = 0; i < parts.size(); i++) {
		total_length += parts[i].size();
	}

	total_length += (parts.size() - 1) * delimiter.size();

	string result(total_length, '\0');
	size_t pos = 0;

	for (size_t i = 0; i < parts.size(); ++i) {
		for (size_t j = 0; j < parts[i].size(); ++j) {
			result[pos++] = parts[i][j];
		}
		if (i < parts.size() - 1) {
			for (size_t j = 0; j < delimiter.size(); ++j) {
				result[pos++] = delimiter[j];
			}
		}
	}
	return result;
}