#ifndef SMART_BOOL_ARRAY_EXCEPTIONS_H
#define SMART_BOOL_ARRAY_EXCEPTIONS_H

#include <stdexcept>

class InvalidArraySizeException final: public std::exception {
public:
	const char* what() const noexcept override {
		return "Error: invalid array size.\n";
	}
};

class IndexOutOfBoundsException final: public std::exception {
public:
	const char* what() const noexcept override {
		return "Error: array index is out of range.\n";
	}
};


#endif //SMART_BOOL_ARRAY_EXCEPTIONS_H

