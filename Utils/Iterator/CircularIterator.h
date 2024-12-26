#pragma once

class CircularIntIterator {
    int* data_m;
    int size_m;
    int* ptr_m;

public:
    CircularIntIterator(int* data, int size);
    void operator++();
    bool operator!=(const CircularIntIterator& other);
    int& operator*() const;
};
