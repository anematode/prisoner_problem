#include <iostream>
#include <bitset>
#include <chrono>

constexpr bool DEBUG = false;

const unsigned int n = 15; // number of wines
const unsigned int m = 9; // number of prisoners

using pattern = u_int16_t; // 16 bit unsigned integer

pattern patterns[n];

int triangle_numbers[n];

pattern consumed[n * (n + 1) / 2];
const pattern max_pattern = (2 << (m - 1)) - 1;

std::string binconv(pattern p) {
    return std::bitset<m>(p).to_string();
}

bool recurse(int depth) {
    if (depth == n) {
        return true;
    }

    for (pattern test_pattern = depth == 0 ? 0 : (patterns[depth - 1] + 1);
         test_pattern <= max_pattern; test_pattern++) {

        if (depth == 0) {
            std::cout << "Depth 0: Pattern " << test_pattern << " of " << max_pattern << ".\n";
        }

        if (DEBUG) std::cout << "Test pattern " << binconv(test_pattern) << " at depth " << depth << ".\n";

        // check if test pattern combines with any other pattern to bring bad news, and remember potential bad news
        if (depth > 0) {
            for (int j = 0; j < depth; j++) { // for all previous patterns
                pattern makes = test_pattern | patterns[j];
                if (DEBUG) std::cout << binconv(makes) << " " << j << " " << depth << ' ' << binconv(test_pattern) << '\n';
                consumed[triangle_numbers[depth - 1] + j] = makes;

                for (int i = 0; i < triangle_numbers[depth - 1] + j; i++) {
                    pattern potential_bad = consumed[i];

                    if (DEBUG) std::cout << binconv(potential_bad) << ' ';

                    if (makes == potential_bad) {
                        goto next_pattern;
                    }
                }

                if (DEBUG) std::cout << '\n';
            }
        }

        patterns[depth] = test_pattern;

        if (recurse(depth + 1))
            return true;

        next_pattern: ;
    }

    return false;
}

int main() {
    // Initialize triangle numbers
    // 0 -> 0, 1 -> 1, 2 -> 3, 3 -> 6, 4 -> 10, 5 -> 15, 6 -> 21

    for (int i = 0; i < n; i++) {
        triangle_numbers[i] = i * (i + 1) / 2;
        patterns[i] = 0; // fill patterns with 0
    }

    auto start = std::chrono::steady_clock::now();

    if (recurse(0)) {
        std::cout << "Solution: \n";

        for (int i = 0; i < n; i++) {
            std::cout << std::bitset<m>(patterns[i]).to_string() << ' ';
        }

        std::cout << std::endl;
    } else {
        std::cout << "No solution found :(\n";
    }

    auto end = std::chrono::steady_clock::now();

    std::cout << "Time elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";
}