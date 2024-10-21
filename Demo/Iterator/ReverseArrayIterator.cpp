#include <iostream>
#include <Iterator/ReverseArrayIterator.h>

int main() {
        int arr[] = {1, 2, 3, 4, 5};
        ReverseArrayIterator rit(arr, sizeof(arr)/sizeof(arr[0]));

        for (auto it = rit.begin(); it != rit.end(); ++it) {
                std::cout << *it << " ";
        }
        // Expected Output: 5 4 3 2 1

        return 0;
}