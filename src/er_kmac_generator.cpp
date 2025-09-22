#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <string>

static constexpr std::size_t HASH_SIZE{48};
static constexpr std::size_t FILENAME_SIZE{12};
static const std::string digits{"123456789ABCDEF"};
constexpr char* const TIME_FORMAT = {"%d.%m.%Y %T "};
constexpr char* const COMPLETE_MSG = {"Generation completed: "};

static std::random_device rd{};
static const std::mt19937 gen(rd());
static std::uniform_int_distribution<int> distribution(0, digits.size() - 1);

static std::ostream& timestamp(std::ostream& stream)
{
    auto raw_time = std::time(nullptr);
    auto tm_struct = *std::localtime(&raw_time);
    const auto time{std::make_unique<tm>(tm_struct)};

    stream << std::put_time(time.get(), TIME_FORMAT);
    return stream;
}

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
        std::cerr << "Failed to write output file\n";
    else
        output_file << timestamp << COMPLETE_MSG << hash << "\n";

    std::clog << timestamp << COMPLETE_MSG << hash << "\n";
}

int main(int, char**)
{
    std::string result{};
    result.reserve(HASH_SIZE);

    _fill_hash(result);

    if (!_is_valid_hash(result))
    {
        std::cerr << "Failed to generate\n";
        std::system("pause");
        std::exit(EXIT_FAILURE);
    }

    _write_hash(result);

    std::system("pause");

    return EXIT_SUCCESS;
}