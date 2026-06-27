#include <algorithm>
#include <exception>
#include <iostream>
#include <string>
#include <unordered_map>

class morze_coder final
{
private:
    static const std::unordered_map<unsigned char, const std::string>
        translation_table;

    morze_coder() = delete;
    morze_coder(const morze_coder&) = delete;
    morze_coder& operator=(const morze_coder&) = delete;
    morze_coder(morze_coder&&) noexcept = delete;
    morze_coder&& operator=(morze_coder&&) noexcept = delete;

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
        std::cout << ex.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
