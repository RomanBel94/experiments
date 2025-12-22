#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <string_view>

static constexpr std::size_t HASH_SIZE{48};
static constexpr std::size_t FILENAME_SIZE{16};
static constexpr std::string_view digits{"123456789ABCDEF"};

static std::random_device rd{};
static const std::mt19937 gen(rd());
static std::uniform_int_distribution<std::size_t>
    distribution(0ull, digits.size() - 1ull);

// Clears parameter and fills it with passed amount
static inline void fill_hash(std::string& input,
                             const std::size_t amount = 0ull) noexcept
{
    input.clear();
    input.reserve(HASH_SIZE);

    for (std::size_t i{0}; i < amount; ++i)
        input.push_back(digits[distribution(rd)]);
}

// Writes string into file
static inline void write_hash_to_file(const std::string& hash) noexcept
{
    std::ofstream output_file{hash.substr(0ull, FILENAME_SIZE) + ".txt",
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

    fill_hash(result, HASH_SIZE);
    write_hash_to_file(result);

    return EXIT_SUCCESS;
}
