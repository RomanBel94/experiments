#include <fstream>
#include <iostream>
#include <random>
#include <string>

class SimpleHashGenerator
{
public:
    SimpleHashGenerator() = default;
    ~SimpleHashGenerator() noexcept = default;

    std::wstring get_hash(std::size_t size);

private:
    static const std::wstring digits;

    static std::random_device rd;
    static const std::mt19937 gen;
    static std::uniform_int_distribution<std::size_t> distribution;

    static void _fill_hash(std::wstring& hash,
                           const std::size_t amount) noexcept;
};

std::wstring SimpleHashGenerator::get_hash(const std::size_t size)
{
    if (size > 0)
    {
        std::wstring result;
        _fill_hash(result, size);
        return result;
    }

    return L"";
}

const std::wstring SimpleHashGenerator::digits{L"123456789ABCDEF"};

std::random_device SimpleHashGenerator::rd{};
const std::mt19937 SimpleHashGenerator::gen(rd());
std::uniform_int_distribution<std::size_t>
    SimpleHashGenerator::distribution(0, digits.size() - 1);

// Clears parameter and fills it with passed amount
void SimpleHashGenerator::_fill_hash(std::wstring& hash,
                                     const std::size_t amount) noexcept
{
    hash.clear();
    hash.reserve(amount);

    for (std::size_t i{0}; i < amount; ++i)
        hash.push_back(digits[distribution(rd)]);
}

int wmain(int argc, wchar_t** argv)
{
#ifdef _WIN32
    std::setlocale(LC_ALL, "ru_RU.UTF-8");
#endif // _WIN32

    if (argc != 2)
    {
        std::wcout << L"Использование: hashgen <размер хеша>\n";
        std::exit(EXIT_FAILURE);
    }
    if (std::stoi(argv[1]) <= 0)
    {
        std::wcerr << L"Размер хеша должен быть больше 0\n";
        std::exit(EXIT_FAILURE);
    }

    SimpleHashGenerator gen;
    std::wstring hash;

    hash = gen.get_hash(std::stoi(argv[1]));

    std::size_t filename_size = hash.size() >= 10 ? 10 : hash.size();
    std::wofstream output_file(hash.substr(0, filename_size) + L".txt",
                               std::ios::out);
    if (!output_file)
    {
        std::wcerr << L"Не удалось записать файл\n";
        std::exit(EXIT_FAILURE);
    }
    output_file << hash;
    std::wcout << L"Готово\n";

    return EXIT_SUCCESS;
}
