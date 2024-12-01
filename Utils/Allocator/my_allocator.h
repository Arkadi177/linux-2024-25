#ifndef MY_ALLOCATOR_H
#define MY_ALLOCATOR_H
#include <iostream>
#include <new>
#include <ostream>

template <class T>
class MyAllocator {
  public:
    using value_type = T;

    MyAllocator() = default;

    template <class U>
    explicit MyAllocator(const MyAllocator<U>& rhs) {}

    T* allocate(size_t size) {
      T* mem = static_cast<T*>(::operator new(size * sizeof(T)));
      if(mem == nullptr) {
        std::cerr << "Out of memory!" << std::endl;
        throw std::bad_alloc();
      }
      return mem;
    }
    void deallocate(T* p , size_t size) {
      ::operator delete(p);
    }

    bool operator==(const MyAllocator<T>& rhs) const {
      return true;
    }
    bool operator!=(const MyAllocator<T>& rhs) const {
      return false;
    }

};
template <class T , class U>
bool operator==(const MyAllocator<T>& lhs , const MyAllocator<U>& rhs) {
  return true;
}

template <class T , class U>
bool operator!=(const MyAllocator<T>& lhs , const MyAllocator<U>& rhs) {
  return false;
}

#endif //MY_ALLOCATOR_H
