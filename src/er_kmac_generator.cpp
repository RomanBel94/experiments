#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

static constexpr std::size_t HASH_SIZE{48};
static constexpr std::size_t FILENAME_SIZE{16};
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

static inline void _write_hash(const std::string& hash)
{
    std::ofstream output_file{hash.substr(0, FILENAME_SIZE) + ".txt",
                              std::ios::out};
    if (!output_file)
    {
        std::cerr << "Failed to write file\n";
        std::exit(EXIT_FAILURE);
    }
    else
    {
        output_file << hash;
        std::clog << "Generation done\n";
    }
}

int main(int, char**)
{
    std::string result{};
    result.reserve(HASH_SIZE);

    _fill_hash(result);

    if (!_is_valid_hash(result))
    {
        std::cerr << "Failed to generate\n";
        std::exit(EXIT_FAILURE);
    }

    _write_hash(result);

    return EXIT_SUCCESS;
}
