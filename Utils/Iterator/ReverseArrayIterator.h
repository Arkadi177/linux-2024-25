#pragma once
class ReverseArrayIterator{
    int* data_m;
    int size_m;
    int* ptr_m;
public:

    ReverseArrayIterator(int* data , int size , int* ptr = nullptr) : size_m(size) , data_m(data) , ptr_m(ptr) {}

    ReverseArrayIterator begin() {
        return {data_m , size_m , data_m + size_m - 1};
    }

    ReverseArrayIterator end() {
        return {data_m , size_m , data_m - 1};
    }

    void operator++() {
        ptr_m--;
    }

    bool operator!=(const ReverseArrayIterator& other) {
        return this->ptr_m != other.ptr_m;
    }

    int& operator*() const {
        return *ptr_m;
    }

};