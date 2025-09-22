#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

static constexpr unsigned int HASH_SIZE{24};
static const std::string digits{"123456789ABCDEF"};

static std::random_device rd{};
static const std::mt19937 gen(rd());
static std::uniform_int_distribution<int> distribution(0, digits.size() - 1);

static inline bool _is_hash(const std::string& hash)
{
    for (auto ch : hash)
        if (!std::isxdigit(ch))
            return false;

    return true;
}

static inline bool _is_valid_hash(const std::string& hash)
{
    return _is_hash(hash) && hash.size() == HASH_SIZE;
}

int main(int, char**)
{
    std::string result{};

    for (std::size_t i{0}; i < HASH_SIZE; ++i)
        result.push_back(digits[distribution(rd)]);

    if (!_is_valid_hash(result))
    {
        std::cerr << "[ERROR] Failed to generate hashkey\n";
        std::exit(EXIT_FAILURE);
    }

    std::ofstream output_file{result + ".txt", std::ios::out};
    if (!output_file)
        std::cerr << "[ERROR] Failed to write output file\n";
    else
        output_file << "[SUCCESS] Hash generation completed: " << result
                    << "\n";

    std::clog << "[SUCCESS] Hash generation completed: " << result << "\n";

    std::system("pause");

    return EXIT_SUCCESS;
}