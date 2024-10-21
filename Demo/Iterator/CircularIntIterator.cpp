#include <iostream>
#include <vector>
#include <Iterator/CircularIntIterator.h>

int main() {
    std::vector<int> numbers = {10, 20, 30};
    CircularIntIterator circular(numbers.data(), numbers.size());

    for (int i = 0; i < 7; ++i) {
        std::cout << *circular << " "; // Access the current element
        ++circular;                    // Move to the next element
    }
    // Expected Output: 10 20 30 10 20 30 10

    return 0;
}