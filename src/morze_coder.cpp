#include <algorithm>
#include <exception>
#include <format>
#include <iostream>
#include <string>
#include <unordered_map>

class NonConstructible
{
private:
    NonConstructible() = delete;
};

class NonCopyable
{
private:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

class NonMovable
{
private:
    NonMovable(NonMovable&&) noexcept = delete;
    NonMovable&& operator=(NonMovable&&) noexcept = delete;
};

class StaticClass : NonConstructible, NonCopyable, NonMovable
{
};

class morze_coder final : StaticClass
{
private:
    static const std::unordered_map<unsigned char, const std::string>
        translation_table;

public:
    static std::string encode(const std::string& str);
};

const std::unordered_map<unsigned char, const std::string>
    morze_coder::translation_table{
        {'a', ".-"},     {'b', "-..."},    {'c', "-.-."},    {'d', "-.."},
        {'e', "."},      {'f', "..-."},    {'g', "--."},     {'h', "...."},
        {'i', ".."},     {'j', ".---"},    {'k', "-.-"},     {'l', ".-.."},
        {'m', "--"},     {'n', "-."},      {'o', "---"},     {'p', ".--."},
        {'q', "--.-"},   {'r', ".-."},     {'s', "..."},     {'t', "-"},
        {'u', "..-"},    {'v', "...-"},    {'w', ".--"},     {'x', "-..-"},
        {'y', "-.--"},   {'z', "--.."},    {'1', ".----"},   {'2', "..---"},
        {'3', "...--"},  {'4', "....-"},   {'5', "....."},   {'6', "-...."},
        {'7', "--..."},  {'8', "---.."},   {'9', "----."},   {'0', "-----"},
        {' ', " "},      {'.', ".-.-.-"},  {',', "--..--"},  {'?', "..--.."},
        {'!', "-.-.--"}, {'\'', ".----."}, {'\"', ".-..-."}, {'(', "-.--."},
        {')', "-.--.-"}, {'&', ".-..."},   {':', "---..."},  {';', "-.-.-."},
        {'/', "-..-."},  {'_', "..--.-"},  {'=', "-...-"},   {'+', ".-.-."},
        {'-', "-....-"}, {'$', "...-..-"}, {'@', ".--.-."}};

std::string morze_coder::encode(const std::string& str)
{
    std::string result;
    result.reserve(str.size() * 6);

    std::ranges::for_each(
        str, [&result](const auto ch)
        { result += translation_table.at(std::tolower(ch)) + ' '; });

    return result;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: morze_coder <message>\n";
        return EXIT_FAILURE;
    }

    try
    {
        std::cout << morze_coder::encode(argv[1]) << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cout << std::format("{}\n", ex.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
