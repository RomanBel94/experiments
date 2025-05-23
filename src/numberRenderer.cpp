#include <array>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

constexpr size_t VERTICAL_SPACE = 1;
constexpr size_t HORIZONTAL_SPACE = 1;
constexpr size_t DIGIT_NUMBER = 3;
constexpr size_t DIGIT_HEIGHT = 9, DIGIT_WIDTH = 8;
constexpr size_t BUFFER_HEIGHT = DIGIT_HEIGHT + 2;
constexpr size_t BUFFER_WIDTH = DIGIT_WIDTH * DIGIT_NUMBER +
                                HORIZONTAL_SPACE * DIGIT_NUMBER +
                                HORIZONTAL_SPACE;

// definitions of digits to draw
// clang-format off
std::array<std::array<char, DIGIT_WIDTH>,DIGIT_HEIGHT> zero{
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

std::array<std::array<char, DIGIT_WIDTH>,DIGIT_HEIGHT> one{
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

std::array<std::array<char, DIGIT_WIDTH>,DIGIT_HEIGHT> two{
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

std::array<std::array<char, DIGIT_WIDTH>,DIGIT_HEIGHT> three{
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

std::array<std::array<char, DIGIT_WIDTH>,DIGIT_HEIGHT> four{
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

std::array<std::array<char, DIGIT_WIDTH>,DIGIT_HEIGHT> five{
    "#######",
    "#      ",
    "#      ",
    "#####  ",
    "#    # ",
    "      #",
    "      #",
    "#    # ",
    " ####  "
};

std::array<std::array<char, DIGIT_WIDTH>,DIGIT_HEIGHT> six{
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

std::array<std::array<char, DIGIT_WIDTH>,DIGIT_HEIGHT> seven{
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

std::array<std::array<char, DIGIT_WIDTH>,DIGIT_HEIGHT> eight{
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

std::array<std::array<char, DIGIT_WIDTH>,DIGIT_HEIGHT> nine{
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

// clang-format on
void write_to_buffer(
    std::array<std::array<char, DIGIT_WIDTH>, DIGIT_HEIGHT> digit,
    size_t buffer_x, size_t buffer_y, char buffer[BUFFER_HEIGHT][BUFFER_WIDTH])
{
    for (size_t i = 0; i < DIGIT_HEIGHT; ++i)
        for (size_t j = 0; j < DIGIT_WIDTH; ++j)
            buffer[buffer_y + i][buffer_x + j] = digit[i][j];
}

bool is_unsigned_integer(char* num)
{
    for (; *num; ++num)
        if (!std::isdigit(*num))
            return false;

    return true;
}

int main(int argc, char* argv[])
{
    // if integer number is not given or too many arguments
    if (argc != 2 || !is_unsigned_integer(argv[1]))
    {
        std::cout << "Usage: numberRenderer <number>\twhere <number> is "
                     "positive number between 0 and 999 including"
                  << std::endl;
        return EXIT_FAILURE;
    }

    // fill map with defined digits
    std::map<char, decltype(zero)> numbers;
    numbers.insert({'0', zero});
    numbers.insert({'1', one});
    numbers.insert({'2', two});
    numbers.insert({'3', three});
    numbers.insert({'4', four});
    numbers.insert({'5', five});
    numbers.insert({'6', six});
    numbers.insert({'7', seven});
    numbers.insert({'8', eight});
    numbers.insert({'9', nine});

    // take a number from arguments
    std::ostringstream number;
    number << std::setw(DIGIT_NUMBER) << std::setfill('0') << argv[1];

    // create and initialize buffer for drawing
    char buffer[BUFFER_HEIGHT][BUFFER_WIDTH];
    std::memset(buffer, ' ', BUFFER_HEIGHT * BUFFER_WIDTH);

    // write each digit to buffer, if numer of digits is greater than 3, first 3
    // digits will be drawn
    for (size_t i = 0; i < DIGIT_NUMBER; ++i)
    {
        write_to_buffer(numbers[number.str()[i]],
                        (DIGIT_WIDTH + HORIZONTAL_SPACE) * i + HORIZONTAL_SPACE,
                        VERTICAL_SPACE, buffer);
    }

    // draw buffer in console
    for (size_t i = 0; i < BUFFER_HEIGHT; ++i)
    {
        for (size_t j = 0; j < BUFFER_WIDTH; ++j)
            std::cout << buffer[i][j];

        std::cout << '\n';
    }

    // clear the output stream
    std::cout.flush();

    return EXIT_SUCCESS;
}
