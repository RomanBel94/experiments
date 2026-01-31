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

struct Config
{
    static constexpr size_t VERTICAL_SPACE = 1;
    static constexpr size_t HORIZONTAL_SPACE = 1;
    static constexpr size_t DIGIT_NUMBER = 12;
    static constexpr size_t DIGIT_HEIGHT = 9, DIGIT_WIDTH = 8;
    static constexpr size_t BUFFER_HEIGHT = DIGIT_HEIGHT + 2;
    static constexpr size_t BUFFER_WIDTH = DIGIT_WIDTH * DIGIT_NUMBER +
                                           HORIZONTAL_SPACE * DIGIT_NUMBER +
                                           HORIZONTAL_SPACE;

    using digit_t = std::array<std::array<char, DIGIT_WIDTH>, DIGIT_HEIGHT>;
    using digit_buffer_t =
        std::array<std::array<char, BUFFER_WIDTH>, BUFFER_HEIGHT>;
};

// definitions of digits to draw
struct char_digits
{
    // clang-format off
constexpr static Config::digit_t zero{
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

constexpr static Config::digit_t one{
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

constexpr static Config::digit_t two{
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

constexpr static Config::digit_t three{
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

constexpr static Config::digit_t four{
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

constexpr static Config::digit_t five{
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

constexpr static Config::digit_t six{
    "  #### ",
    " #     ",
    "#      ",
    "# #### ",
    "##    #",
    "#     #",
    "#     #",
    "#    # ",
    " ####  "
};

constexpr static Config::digit_t seven{
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

constexpr static Config::digit_t eight{
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

constexpr static Config::digit_t nine{
    "  #### ",
    " #    #",
    "#     #",
    "#     #",
    "#    ##",
    " #### #",
    "      #",
    "     # ",
    " ####  "
};
};

// clang-format on

class Renderer final
{
public:
    Config::digit_buffer_t render(unsigned long num)
    {
        Config::digit_buffer_t buffer;
        _init_buffer(buffer);
        std::string number = std::format("{:0>12}", num);
        _write_buffer(buffer, number);
        return buffer;
    }

    Config::digit_buffer_t render(std::string_view num)
    {
        Config::digit_buffer_t buffer;
        _init_buffer(buffer);
        std::string number = std::format("{:0>12}", num);
        _write_buffer(buffer, number);
        return buffer;
    }

    static void display(Config::digit_buffer_t const& buf)
    {
        // draw buffer in console
        for (const auto& row : buf)
        {
            for (const auto ch : row)
                std::cout << ch;

            std::cout << '\n';
        }
    }

private:
    // fill map with defined digits
    inline static const std::unordered_map<char, Config::digit_t> numbers{
        {'0', char_digits::zero},  {'1', char_digits::one},
        {'2', char_digits::two},   {'3', char_digits::three},
        {'4', char_digits::four},  {'5', char_digits::five},
        {'6', char_digits::six},   {'7', char_digits::seven},
        {'8', char_digits::eight}, {'9', char_digits::nine}};

    void _init_buffer(Config::digit_buffer_t& buf)
    {
        for (auto& row : buf)
            for (auto& ch : row)
                ch = ' ';
    }

    void _write_buffer(Config::digit_buffer_t& buf, std::string_view num)
    {
        for (size_t i = 0; i < Config::DIGIT_NUMBER; ++i)
            _write_digit_to_buffer(
                numbers.at(num[i]),
                (Config::DIGIT_WIDTH + Config::HORIZONTAL_SPACE) * i +
                    Config::HORIZONTAL_SPACE,
                Config::VERTICAL_SPACE, buf);
    }

    void _write_digit_to_buffer(Config::digit_t const& digit,
                                size_t buffer_x_offset, size_t buffer_y_offset,
                                Config::digit_buffer_t& buffer)
    {
        for (size_t i = 0; i < Config::DIGIT_HEIGHT; ++i)
            for (size_t j = 0; j < Config::DIGIT_WIDTH; ++j)
                buffer[buffer_y_offset + i][buffer_x_offset + j] = digit[i][j];
    }
};

bool is_unsigned_integer(std::string_view num)
{
    for (auto digit : num)
        if (!std::isdigit(digit))
            return false;

    return true;
}

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

    Renderer render;

    auto num = render.render(argv[1]);
    render.display(num);

    return EXIT_SUCCESS;
}
