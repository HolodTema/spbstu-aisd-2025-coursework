#include "SmartBoolArray.h"

SmartBoolArray& SmartBoolArray::operator=(SmartBoolArray& other) {
	if (&other != this) {

		SmartBoolArray otherCopy(other);
		swap(other);
	}
	return *this;
}

SmartBoolArray& SmartBoolArray::operator=(SmartBoolArray&& other) noexcept {
	size_ = other.size_;
	capacity_ = other.capacity_;
	array_ = other.array_;
	other.array_ = nullptr;
	return *this;
}

bool SmartBoolArray::get(int index) const {
	if (index >= size_ || index < 0) {
		throw IndexOutOfBoundsException();
	}
	unsigned char ch = array_[index / 8];
	unsigned int indexInChar = index - (index / 8) * 8;

	unsigned char multiplier = 128 / (1 << indexInChar);

	return ((ch & multiplier) >> (7 - indexInChar)) == 1 ? true : false;
}

void SmartBoolArray::set(int index, bool value) {
	if (index >= size_ || index < 0) {
		throw IndexOutOfBoundsException();
	}
	unsigned char ch = array_[index / 8];
	unsigned int indexInChar = index - (index / 8) * 8;
	if (value) {
		unsigned char multiplier = (1 << (7-indexInChar));
		ch = ch | multiplier;
		array_[index / 8] = ch;
	}
	else {
		unsigned char multiplier = 255 - (1 << (7-indexInChar));
		ch = ch & multiplier;
		array_[index / 8] = ch;
	}
}

int SmartBoolArray::getSize() const noexcept {
	return size_;
}

bool *SmartBoolArray::toBoolArray() const {
	bool* result = new bool[size_];
	for (int i = 0; i < size_; ++i) {
		result[i] = get(i);
	}
	return result;
}


void SmartBoolArray::swap(SmartBoolArray& other) noexcept {
	std::swap(array_, other.array_);
	std::swap(capacity_, other.capacity_);
	std::swap(size_, other.size_);
}

std::ostream& operator<<(std::ostream& os, const SmartBoolArray& array) {
	for (int i = 0; i < array.getSize(); ++i) {
		os << array.get(i) << " ";
	}
	os << "\n";
	return os;
}



