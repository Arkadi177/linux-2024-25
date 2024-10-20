#pragma once
#include <functional>
#include <utility>
class FilterIntegerIterator {
public:

    FilterIntegerIterator(int *data, int size, std::function<bool(int)> other_condition) : ptr_m(data), size_m(size) , data_m(data)
    , condition(std::move(other_condition)) {}

    FilterIntegerIterator(int *data, int size, int *ptr) : size_m(size), data_m(data), ptr_m(ptr)  {}

    FilterIntegerIterator begin() {
        FilterIntegerIterator tmp(data_m, size_m, data_m);
        tmp.condition = condition;
        while(!condition(*tmp.ptr_m) && tmp.ptr_m < tmp.ptr_m + tmp.size_m) {
            tmp.ptr_m++;
        }
        return tmp;
    }

    FilterIntegerIterator end() {
        FilterIntegerIterator tmp(data_m, size_m, data_m + size_m );
        tmp.condition = condition;
        return tmp;
    }

    void operator++() {
        do {
            ++ptr_m;
        } while (ptr_m != data_m + size_m && !condition(*ptr_m));
    }

    bool operator!=(const FilterIntegerIterator &other) {
        return this->ptr_m != other.ptr_m;
    }

    int &operator*() const {
        return *ptr_m;
    }

private:
    int *ptr_m;
    int size_m;
    int *data_m;
    std::function<bool(int)> condition;

};