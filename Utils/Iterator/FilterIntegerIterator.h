#ifndef FILTER_INTEGER_ITERATOR_H

#define FILTER_INTEGER_ITERATOR_H
#include <functional>
#include <iostream>
#include <utility>
class FilterIntegerIterator {
public:
    FilterIntegerIterator(int *data, int size, std::function<bool(int)> other_condition);
    FilterIntegerIterator(int *data, int size, int *ptr);
    FilterIntegerIterator begin();
    FilterIntegerIterator end();

    void operator++();
    bool operator!=(const FilterIntegerIterator &other);
    int &operator*() const;

private:
    int *ptr_m;
    int size_m;
    int *data_m;
    std::function<bool(int)> condition;
};
FilterIntegerIterator::FilterIntegerIterator(int *data, int size, std::function<bool(int)> other_condition)
        : ptr_m(data), size_m(size) , data_m(data) , condition(std::move(other_condition))
{}
FilterIntegerIterator::FilterIntegerIterator(int *data, int size, int *ptr)
        : size_m(size), data_m(data), ptr_m(ptr) {}

FilterIntegerIterator FilterIntegerIterator::begin() {
    FilterIntegerIterator tmp(data_m, size_m, data_m);
    tmp.condition = condition;
    while(!condition(*tmp.ptr_m) && tmp.ptr_m < tmp.ptr_m + tmp.size_m) {
        tmp.ptr_m++;
    }
    return tmp;
}
FilterIntegerIterator FilterIntegerIterator::end() {
    FilterIntegerIterator tmp(data_m, size_m, data_m + size_m );
    tmp.condition = condition;
    return tmp;
}
void FilterIntegerIterator::operator++() {
    do {
        ++ptr_m;
    } while (ptr_m != data_m + size_m && !condition(*ptr_m));
}
bool FilterIntegerIterator::operator!=(const FilterIntegerIterator &other) {
    return this->ptr_m != other.ptr_m;
}
int &FilterIntegerIterator::operator*() const {
    return *ptr_m;
}
#endif // FILTER_INTEGER_ITERATOR_H