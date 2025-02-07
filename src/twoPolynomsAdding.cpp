#include <fstream>
#include <iostream>
#include <map>
#include <string>

int main()
{
    std::ifstream input_file("polynoms.txt", std::ios::in);

    if (!input_file.is_open())
    {
        std::cout << "Can't open input file!" << std::endl;
        return -1;
    }

    std::map<int, int> first_polynom{};
    std::map<int, int> second_polynom{};
    std::map<int, int> result_polynom{};
    std::map<int, int>* current_polynom = &first_polynom;

    std::string number_buffer;
    std::string power_buffer;

    while (!input_file.eof())
    {
        // go to first number
        while (std::isspace(input_file.peek()))
            input_file.get();
        // read first number
        while (std::isdigit(input_file.peek()))
            power_buffer += input_file.get();
        // go to second number
        while (std::isspace(input_file.peek()))
            input_file.get();
        // read second number
        while (std::isdigit(input_file.peek()))
            number_buffer += input_file.get();
        // go to end of line
        while (std::isspace(input_file.peek()) && input_file.peek() != '\n')
            input_file.get();

        current_polynom->insert(
            {std::stoi(number_buffer), std::stoi(power_buffer)});
        number_buffer.clear();
        power_buffer.clear();

        if (input_file.get() == '\n' && input_file.peek() == '\n')
            current_polynom = &second_polynom;
    }

    int iterations = std::max(first_polynom.size(), second_polynom.size());

    // adding polynoms
    for (int i = 0; i <= iterations; ++i)
    {
        result_polynom[i] = first_polynom[i] + second_polynom[i];
        std::cout << "Number " << result_polynom[i] << " power " << i << '\n';
    }        

    return 0;
}