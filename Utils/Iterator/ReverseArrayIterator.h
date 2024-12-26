#pragma once

class ReverseArrayIterator {
    int* data_m;
    int size_m;
    int* ptr_m;

public:
    ReverseArrayIterator(int* data, int size, int* ptr = nullptr);

    ReverseArrayIterator begin();
    ReverseArrayIterator end();

    void operator++();
    bool operator!=(const ReverseArrayIterator& other);
    int& operator*() const;
};
