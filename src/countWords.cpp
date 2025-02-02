#include <iostream>
#include <sstream>
#include <string>

unsigned int countWords(std::string& str)
{
    std::istringstream stream(str);
    std::string word;
    unsigned int count{0};

    while (stream >> word)
        ++count;
    return count;
}

int main()
{
    std::string expr1 = "Hello World!";
    std::string expr2 = "Some string      with      some  words...";

    std::cout << countWords(expr1) << std::endl;
    std::cout << countWords(expr2) << std::endl;

    return 0;
}
