#ifndef FILTER_INTEGER_ITERATOR_H

#define FILTER_INTEGER_ITERATOR_H

class FilterIntegerIterator {
public:
    FilterIntegerIterator(int *data, int size, bool (*condition)(int));
    FilterIntegerIterator(int *data, int size, int *ptr);
    ~FilterIntegerIterator();

    FilterIntegerIterator begin();
    FilterIntegerIterator end();

    void operator++();
    bool operator!=(const FilterIntegerIterator &other);
    int &operator*() const;

private:
    int *ptr_m;
    int size_m;
    int *data_m;
};
FilterIntegerIterator::FilterIntegerIterator(int *data, int size, bool (*condition)(int))
        : ptr_m(nullptr), size_m(0) , data_m(new int[size]) {
    int k = 0;
    for (int i = 0; i < size; ++i) {
        if (condition(data[i])) {
            data_m[k++] = data[i];
            size_m++;
        }
    }
}
FilterIntegerIterator::FilterIntegerIterator(int *data, int size, int *ptr)
        : size_m(size), data_m(new int[size]), ptr_m(ptr) {}

FilterIntegerIterator::~FilterIntegerIterator() {
    delete[] data_m;
}

FilterIntegerIterator FilterIntegerIterator::begin() {
    return FilterIntegerIterator(data_m, size_m, data_m);
}

FilterIntegerIterator FilterIntegerIterator::end() {
    return FilterIntegerIterator(data_m, size_m, data_m + size_m);
}

void FilterIntegerIterator::operator++() {
    ptr_m++;
}

bool FilterIntegerIterator::operator!=(const FilterIntegerIterator &other) {
    return this->ptr_m != other.ptr_m;
}
int &FilterIntegerIterator::operator*() const {
    return *ptr_m;
}
#endif // FILTER_INTEGER_ITERATOR_H