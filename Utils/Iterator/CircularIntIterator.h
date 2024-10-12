#pragma once
class CircularIntIterator{
    int* data_m;
    int size_m;
    int* ptr_m;
public:
    CircularIntIterator(int* , int);
    void operator++();
    bool operator!=(const CircularIntIterator&);
    int& operator*() const;
};
CircularIntIterator::CircularIntIterator(int* data , int size)
        : size_m(size) , data_m(data) , ptr_m(data){
}
void CircularIntIterator::operator++() {
    if(ptr_m == size_m + data_m - 1)
    {
        ptr_m = data_m;
    }
    else {
        ptr_m++;
    }
}
bool CircularIntIterator::operator!=(const CircularIntIterator &other) {
    return this->ptr_m != other.ptr_m;
}
int &CircularIntIterator::operator*() const {
    return *ptr_m;
}