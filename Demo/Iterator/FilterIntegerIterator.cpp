

#include <iostream>
#include <vector>
#include <Iterator/FilterIntegerIterator.h>

bool IsEven(int num) {
    return num % 2 == 0;
}

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6};
    FilterIntegerIterator evenIter(numbers.data(), numbers.size(), IsEven);

    for (auto it = evenIter.begin(); it != evenIter.end(); ++it) {
        std::cout << *it << " ";
    }
    // Expected Output: 2 4 6

    return 0;
}