#include <concepts>
#include <iostream>
#include <span>
#include <vector>

template <typename T>
    requires std::integral<T>
void print_elements(std::span<const T> container) {
    for (const auto& item : container) {
        std::cout << item << ' ';
    }
    std::cout << '\n';
}

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    std::cout << "Testing C++23 features:\n";
    print_elements<int>(numbers);

    return 0;
}
