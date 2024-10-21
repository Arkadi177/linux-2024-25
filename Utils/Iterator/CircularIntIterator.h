#pragma once
class CircularIntIterator{
    int* data_m;
    int size_m;
    int* ptr_m;
public:
    CircularIntIterator(int* data, int size) : size_m(size) , data_m(data) , ptr_m(data) {}
    void operator++() {
        ptr_m = (ptr_m == size_m + data_m - 1)? data_m : ptr_m + 1;
    }
    bool operator!=(const CircularIntIterator& other) {
        return this->ptr_m != other.ptr_m;
    }
    int& operator*() const {
        return *ptr_m;
    }
};