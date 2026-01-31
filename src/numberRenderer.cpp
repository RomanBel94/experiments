/**
 * @file numberRenderer.cpp
 *
 * @author Roman Belyaev
 *
 * @brief Renders ascii number from integer
 *
 * @todo Incapsulate all this shit and make API
 */
#include <array>
#include <cstring>
#include <format>
#include <iostream>
#include <string_view>
#include <unordered_map>

constexpr size_t VERTICAL_SPACE = 1;
constexpr size_t HORIZONTAL_SPACE = 1;
constexpr size_t DIGIT_NUMBER = 12;
constexpr size_t DIGIT_HEIGHT = 9, DIGIT_WIDTH = 8;
constexpr size_t BUFFER_HEIGHT = DIGIT_HEIGHT + 2;
constexpr size_t BUFFER_WIDTH = DIGIT_WIDTH * DIGIT_NUMBER +
                                HORIZONTAL_SPACE * DIGIT_NUMBER +
                                HORIZONTAL_SPACE;

using digit_t = std::array<std::array<char, DIGIT_WIDTH>, DIGIT_HEIGHT>;
using digit_buffer_t =
    std::array<std::array<char, BUFFER_WIDTH>, BUFFER_HEIGHT>;

struct char_digits
{
    // definitions of digits to draw
    // clang-format off
digit_t zero{
    "  ###  ",
    " #   # ",
    "#     #",
    "#     #",
    "#     #",
    "#     #",
    "#     #",
    " #   # ",
    "  ###  "
};

digit_t one{
    "   #   ",
    "  ##   ",
    " # #   ",
    "#  #   ",
    "   #   ",
    "   #   ",
    "   #   ",
    "   #   ",
    " ##### "
};

digit_t two{
    "  ###  ",
    " #   # ",
    "#     #",
    "     # ",
    "    #  ",
    "   #   ",
    "  #    ",
    " #     ",
    "#######"
};

digit_t three{
    " ####  ",
    "#    # ",
    "#     #",
    "     # ",
    "    #  ",
    "     # ",
    "#     #",
    "#    # ",
    " ####  "
};

digit_t four{
    "     # ",
    "    ## ",
    "   # # ",
    "  #  # ",
    " #   # ",
    "#######",
    "     # ",
    "     # ",
    "     # "
};

digit_t five{
    " ######",
    "#      ",
    "#      ",
    "#####  ",
    "     # ",
    "      #",
    "      #",
    "#    # ",
    " ####  "
};

digit_t six{
    "  #### ",
    " #    #",
    "#      ",
    "# #### ",
    "##    #",
    "#     #",
    "#     #",
    " #    #",
    "  #### "
};

digit_t seven{
    "#######",
    "     # ",
    "    #  ",
    "   #   ",
    "   #   ",
    "   #   ",
    "   #   ",
    "   #   ",
    "   #   "
};

digit_t eight{
    "  ###  ",
    " #   # ",
    " #   # ",
    "  ###  ",
    " #   # ",
    "#     #",
    "#     #",
    " #   # ",
    "  ###  "
};

digit_t nine{
    " ####  ",
    "#    # ",
    "#     #",
    "#     #",
    "#    ##",
    " #### #",
    "      #",
    "#    # ",
    " ####  "
};
} digits;

// clang-format on
void write_to_buffer(digit_t digit, size_t buffer_x_offset,
                     size_t buffer_y_offset, digit_buffer_t& buffer)
{
    for (size_t i = 0; i < DIGIT_HEIGHT; ++i)
        for (size_t j = 0; j < DIGIT_WIDTH; ++j)
            buffer[buffer_y_offset + i][buffer_x_offset + j] = digit[i][j];
}

bool is_unsigned_integer(std::string_view num)
{
    for (auto digit : num)
        if (!std::isdigit(digit))
            return false;

    return true;
}

// fill map with defined digits
const std::unordered_map<char, digit_t> numbers{
    {'0', digits.zero},  {'1', digits.one},   {'2', digits.two},
    {'3', digits.three}, {'4', digits.four},  {'5', digits.five},
    {'6', digits.six},   {'7', digits.seven}, {'8', digits.eight},
    {'9', digits.nine}};

int main(int argc, char* argv[])
{
    // if integer number is not given or too many arguments
    if (argc != 2 || !is_unsigned_integer(argv[1]))
    {
        std::cout << "Usage: numberRenderer <number>\twhere <number> is "
                     "unsigned integer"
                  << std::endl;
        return EXIT_FAILURE;
    }

    // take a number from arguments
    std::string number = std::format("{:0>12}", argv[1]);

    // create and initialize buffer for drawing
    digit_buffer_t buffer;

    for (auto& row : buffer)
        for (auto& ch : row)
            ch = ' ';

    // write each digit to buffer, if numer of digits is greater than 3, first 3
    // digits will be drawn
    for (size_t i = 0; i < DIGIT_NUMBER; ++i)
        write_to_buffer(numbers.at(number[i]),
                        (DIGIT_WIDTH + HORIZONTAL_SPACE) * i + HORIZONTAL_SPACE,
                        VERTICAL_SPACE, buffer);

    // draw buffer in console
    for (const auto& row : buffer)
    {
        for (const auto ch : row)
            std::cout << ch;

        std::cout << '\n';
    }

    return EXIT_SUCCESS;
}
