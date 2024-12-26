#include "CircularIterator.h"

CircularIntIterator::CircularIntIterator(int* data, int size)
    : size_m(size), data_m(data), ptr_m(data) {}

void CircularIntIterator::operator++() {
  ptr_m = (ptr_m == size_m + data_m - 1) ? data_m : ptr_m + 1;
}

bool CircularIntIterator::operator!=(const CircularIntIterator& other) {
  return this->ptr_m != other.ptr_m;
}

int& CircularIntIterator::operator*() const {
  return *ptr_m;
}
