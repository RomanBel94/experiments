#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

unsigned int countWords(const std::string& str)
{
    std::cout << "countWords(const std::string& str)" << std::endl;
    std::istringstream stream(str);
    std::string word;
    unsigned int count{0};

    while (stream >> word)
        ++count;
    return count;
}

unsigned int countWords(const std::string& str, bool notused)
{
    std::cout << "countWords(const std::string& str, bool notused)"
              << std::endl;
    auto it = str.begin();
    unsigned int count{0};

    while (it != str.end())
    {
        while (it != str.end() && std::isspace(*it))
            ++it;
        while (it != str.end() && !std::isspace(*it))
            ++it;
        ++count;
    }
    return count;
}

int main()
{
    std::string expr1 = "Hello World!";
    std::string expr2 = "Some string      with      some  words...";

    std::cout << countWords(expr1) << std::endl;
    std::cout << countWords(expr2) << std::endl;

    std::cout << countWords(expr1, true) << std::endl;
    std::cout << countWords(expr2, true) << std::endl;

    return 0;
}
