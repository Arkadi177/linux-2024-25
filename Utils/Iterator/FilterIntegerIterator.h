#pragma once
#include <functional>
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
