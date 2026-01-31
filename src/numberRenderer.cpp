/**
 * @file numberRenderer.cpp
 *
 * @author Roman Belyaev
 *
 * @brief Renders ascii number from integer
 *
 * @todo Incapsulate all this shit and make API
 * @todo MORE STD!!!
 */
#include <algorithm>
#include <array>
#include <cctype>
#include <cstring>
#include <format>
#include <iostream>
#include <iterator>
#include <string_view>
#include <unordered_map>

class Renderer final
{
private:
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

    // clang-format off
// definitions of digits to draw
struct char_digits
{
static constexpr  Config::digit_t zero{
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

static constexpr  Config::digit_t one{
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

static constexpr  Config::digit_t two{
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

static constexpr  Config::digit_t three{
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

static constexpr  Config::digit_t four{
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

static constexpr  Config::digit_t five{
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

static constexpr  Config::digit_t six{
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

static constexpr  Config::digit_t seven{
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

static constexpr  Config::digit_t eight{
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

static constexpr  Config::digit_t nine{
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

    // draw buffer in console
    static void display(Config::digit_buffer_t const& buf)
    {
        std::ranges::for_each(
            buf,
            [](const auto& row)
            {
                std::ranges::copy(row, std::ostream_iterator<char>(std::cout));
                std::cout << '\n';
            });
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
        std::ranges::for_each(buf,
                              [](auto& row) { std::ranges::fill(row, ' '); });
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

int main(int argc, char* argv[])
{
    // if integer number is not given or too many arguments
    if (argc != 2 || !std::ranges::all_of(std::string_view(argv[1]), [](char i)
                                          { return std::isdigit(i); }))
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
