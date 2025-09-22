#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

static constexpr std::size_t HASH_SIZE{48};
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

static inline void _fill_hash(std::string& hash)
{
    for (std::size_t i{0}; i < HASH_SIZE; ++i)
        hash.push_back(digits[distribution(rd)]);
}

static inline void _write_hash(const std::string& result)
{
    std::ofstream output_file{result + ".txt", std::ios::out};
    if (!output_file)
        std::cerr << "[ERROR] Failed to write output file\n";
    else
        output_file << "[SUCCESS] Hash generation completed: " << result
                    << "\n";

    std::clog << "[SUCCESS] Hash generation completed: " << result << "\n";
}

int main(int, char**)
{
    std::string result{};
    result.reserve(HASH_SIZE);

    _fill_hash(result);

    if (!_is_valid_hash(result))
    {
        std::cerr << "[ERROR] Failed to generate hashkey\n";
        std::system("pause");
        std::exit(EXIT_FAILURE);
    }

    _write_hash(result);

    std::system("pause");

    return EXIT_SUCCESS;
}