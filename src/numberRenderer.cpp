#include <array>
#include <iostream>
#include <string>
#include <unordered_map>

constexpr size_t HEIGHT = 9, WIDTH = 8;
constexpr size_t BUFFER_HEIGHT = HEIGHT + 2;
constexpr size_t BUFFER_WIDTH = WIDTH * 3 + 4;

// clang-format off
std::array<std::array<char, WIDTH>,HEIGHT> zero{
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

std::array<std::array<char, WIDTH>,HEIGHT> one{
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

std::array<std::array<char, WIDTH>,HEIGHT> two{
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

std::array<std::array<char, WIDTH>,HEIGHT> three{
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

std::array<std::array<char, WIDTH>,HEIGHT> four{
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

std::array<std::array<char, WIDTH>,HEIGHT> five{
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

std::array<std::array<char, WIDTH>,HEIGHT> six{
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

std::array<std::array<char, WIDTH>,HEIGHT> seven{
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

std::array<std::array<char, WIDTH>,HEIGHT> eight{
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

std::array<std::array<char, WIDTH>,HEIGHT> nine{
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
void write_to_buffer(std::array<std::array<char, WIDTH>,HEIGHT> digit, size_t buffer_x, size_t buffer_y,
                     char buffer[BUFFER_HEIGHT][BUFFER_WIDTH])
{
    for (size_t i = 0; i < HEIGHT; ++i)
        for (size_t j = 0; j < WIDTH; ++j)
            buffer[buffer_y + i][buffer_x + j] = digit[i][j];
}

int main(int argc, char* argv[]) {
     if (argc == 1) {
         std::cout << "Usage: numberRenderer <number>\twhere <number> is "
                      "positive number between 0 and 999 including"
                   << std::endl;
         return EXIT_FAILURE;
     }

    std::unordered_map<char, decltype(zero)> numbers;
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

    std::string number{argv[1]};

    if (number.size() == 1)
        number = "00" + number;
    else if (number.size() == 2)
        number = '0' + number;
    else if (number.size() > 3) {
        std::cout << "Usage: numberRenderer <number>\twhere <number> is "
                     "positive number between 0 and 999 including"
                  << std::endl;
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_HEIGHT][BUFFER_WIDTH]{};

    for (size_t i = 0; i < 3; ++i) {
        write_to_buffer(numbers[number[i]], (WIDTH + 1) * i + 1, 1,  buffer);
    }

    for (size_t i = 0; i < BUFFER_HEIGHT; ++i) {
        for (size_t j = 0; j < BUFFER_WIDTH; ++j)
            std::cout << buffer[i][j];

        std::cout << '\n';
    }

    return EXIT_SUCCESS;
}
