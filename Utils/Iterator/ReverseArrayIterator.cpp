#include "ReverseArrayIterator.h"

ReverseArrayIterator::ReverseArrayIterator(int* data, int size, int* ptr)
    : size_m(size), data_m(data), ptr_m(ptr) {}

ReverseArrayIterator ReverseArrayIterator::begin() {
  return {data_m, size_m, data_m + size_m - 1};
}

ReverseArrayIterator ReverseArrayIterator::end() {
  return {data_m, size_m, data_m - 1};
}

void ReverseArrayIterator::operator++() {
  ptr_m--;
}

bool ReverseArrayIterator::operator!=(const ReverseArrayIterator& other) {
  return this->ptr_m != other.ptr_m;
}

int& ReverseArrayIterator::operator*() const {
  return *ptr_m;
}
