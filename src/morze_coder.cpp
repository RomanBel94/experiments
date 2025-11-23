#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>

class morze_coder final
{
private:
    static const std::unordered_map<unsigned char, const std::string>
        translation_table;

public:
    static std::string encode(const std::string& str);
};

const std::unordered_map<unsigned char, const std::string>
    morze_coder::translation_table{
        {'a', ".-"},    {'b', "-..."},  {'c', "-.-."},  {'d', "-.."},
        {'e', "."},     {'f', "..-."},  {'g', "--."},   {'h', "...."},
        {'i', ".."},    {'j', ".---"},  {'k', "-.-"},   {'l', ".-.."},
        {'m', "--"},    {'n', "-."},    {'o', "---"},   {'p', ".--."},
        {'q', "--.-"},  {'r', ".-."},   {'s', "..."},   {'t', "-"},
        {'u', "..-"},   {'v', "...-"},  {'w', ".--"},   {'x', "-..-"},
        {'y', "-.--"},  {'z', "--.."},  {'1', ".----"}, {'2', "..---"},
        {'3', "...--"}, {'4', "....-"}, {'5', "....."}, {'6', "-...."},
        {'7', "--..."}, {'8', "---.."}, {'9', "----."}, {'0', "-----"},
        {' ', " "}};

std::string morze_coder::encode(const std::string& str)
{
    std::string result{};

    std::for_each(
        str.cbegin(), str.cend(),
        [&result](const auto ch) {
            result += morze_coder::translation_table.at(std::tolower(ch)) + ' ';
        });

    return result;
}

int main(int argc, char* argv[])
{
    std::cout << morze_coder::encode("Hello world") << std::endl;
    return EXIT_SUCCESS;
}
