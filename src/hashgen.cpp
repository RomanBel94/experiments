#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

class SimpleHashGenerator
{
public:
    SimpleHashGenerator() = default;
    ~SimpleHashGenerator() noexcept = default;

    std::string get_hash(std::size_t size);

private:
    static const std::string digits;

    static std::random_device rd;
    static const std::mt19937 gen;
    static std::uniform_int_distribution<int> distribution;

    static void _fill_hash(std::string& hash, std::size_t amount) noexcept;
};

std::string SimpleHashGenerator::get_hash(std::size_t size)
{
    if (size > 0)
    {
        std::string result;
        _fill_hash(result, size);
        return result;
    }
    else
        throw std::runtime_error{"null hash size"};
}

const std::string SimpleHashGenerator::digits{"123456789ABCDEF"};

std::random_device SimpleHashGenerator::rd{};
const std::mt19937 SimpleHashGenerator::gen(rd());
std::uniform_int_distribution<int>
    SimpleHashGenerator::distribution(0, digits.size() - 1);

// Clears parameter and fills it with passed amount
inline void SimpleHashGenerator::_fill_hash(std::string& hash,
                                            std::size_t amount) noexcept
{
    hash.clear();
    hash.reserve(amount);

    for (std::size_t i{0}; i < amount; ++i)
        hash.push_back(digits[distribution(rd)]);
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "usage: hashgen hash_size\n";
        std::exit(EXIT_FAILURE);
    }

    SimpleHashGenerator gen;
    std::string res;
    try
    {
        res = gen.get_hash(std::stoi(argv[1]));
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::size_t filename_size = res.size() > 10 ? 10 : res.size();
    std::ofstream output_file(res.substr(0, filename_size) + ".txt",
                              std::ios::out);
    if (!output_file)
    {
        std::cout << "Can't write file\n";
        std::exit(EXIT_FAILURE);
    }
    output_file << res;
    std::cout << "done\n";

    return EXIT_SUCCESS;
}
