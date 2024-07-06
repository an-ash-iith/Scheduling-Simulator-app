#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<std::pair<int, int>> vec = {{3, 4}, {2, 3}, {3, 2}, {1, 5}, {2, 2}};
    
    // Sort the vector of pairs
    std::sort(vec.begin(), vec.end());

    // Print the sorted vector
    for (const auto& p : vec) {
        std::cout << "(" << p.first << ", " << p.second << ") ";
    }
    
    return 0;
}
