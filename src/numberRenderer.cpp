/**
 * @file numberRenderer.cpp
 *
 * @author Roman Belyaev
 *
 * @brief Renders ascii number from integer
 *
 * @todo MORE STD!!!
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <source_location>
#include <stdexcept>
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
        static constexpr auto DIGIT_FORMAT = "{:0>12}";
        static constexpr size_t DIGIT_HEIGHT = 9, DIGIT_WIDTH = 7;
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
static constexpr Config::digit_t zero{
       {{' ', ' ', '#', '#', '#', ' ', ' '},
        {' ', '#', ' ', ' ', ' ', '#', ' '},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {' ', '#', ' ', ' ', ' ', '#', ' '},
        {' ', ' ', '#', '#', '#', ' ', ' '}},
};

static constexpr Config::digit_t one{
       {{' ', ' ', ' ', '#', ' ', ' ', ' '},
        {' ', ' ', '#', '#', ' ', ' ', ' '},
        {' ', '#', ' ', '#', ' ', ' ', ' '},
        {'#', ' ', ' ', '#', ' ', ' ', ' '},
        {' ', ' ', ' ', '#', ' ', ' ', ' '},
        {' ', ' ', ' ', '#', ' ', ' ', ' '},
        {' ', ' ', ' ', '#', ' ', ' ', ' '},
        {' ', ' ', ' ', '#', ' ', ' ', ' '},
        {' ', '#', '#', '#', '#', '#', ' '}}
};

static constexpr Config::digit_t two{
       {{' ', ' ', '#', '#', '#', ' ', ' '},
        {' ', '#', ' ', ' ', ' ', '#', ' '},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {' ', ' ', ' ', ' ', ' ', '#', ' '},
        {' ', ' ', ' ', ' ', '#', ' ', ' '},
        {' ', ' ', ' ', '#', ' ', ' ', ' '},
        {' ', ' ', '#', ' ', ' ', ' ', ' '},
        {' ', '#', ' ', ' ', ' ', ' ', ' '},
        {'#', '#', '#', '#', '#', '#', '#'}}
};

static constexpr Config::digit_t three{
       {{' ', '#', '#', '#', '#', ' ', ' '},
        {'#', ' ', ' ', ' ', ' ', '#', ' '},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {' ', ' ', ' ', ' ', ' ', '#', ' '},
        {' ', ' ', ' ', ' ', '#', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', '#', ' '},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', '#', ' '},
        {' ', '#', '#', '#', '#', ' ', ' '}}
};

static constexpr Config::digit_t four{
       {{' ', ' ', ' ', ' ', ' ', '#', ' '},
        {' ', ' ', ' ', ' ', '#', '#', ' '},
        {' ', ' ', ' ', '#', ' ', '#', ' '},
        {' ', ' ', '#', ' ', ' ', '#', ' '},
        {' ', '#', ' ', ' ', ' ', '#', ' '},
        {'#', '#', '#', '#', '#', '#', '#'},
        {' ', ' ', ' ', ' ', ' ', '#', ' '},
        {' ', ' ', ' ', ' ', ' ', '#', ' '},
        {' ', ' ', ' ', ' ', ' ', '#', ' '}}
};

static constexpr Config::digit_t five{
       {{' ', '#', '#', '#', '#', '#', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' '},
        {'#', ' ', ' ', ' ', ' ', ' ', ' '},
        {'#', '#', '#', '#', '#', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', '#', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', '#', ' '},
        {' ', '#', '#', '#', '#', ' ', ' '}}
};

static constexpr Config::digit_t six{
       {{' ', ' ', '#', '#', '#', '#', ' '},
        {' ', '#', ' ', ' ', ' ', ' ', ' '},
        {'#', ' ', ' ', ' ', ' ', ' ', ' '},
        {'#', ' ', '#', '#', '#', '#', ' '},
        {'#', '#', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', '#', ' '},
        {' ', '#', '#', '#', '#', ' ', ' '}}
};

static constexpr Config::digit_t seven{
       {{'#', '#', '#', '#', '#', '#', '#'},
        {' ', ' ', ' ', ' ', ' ', '#', ' '},
        {' ', ' ', ' ', ' ', '#', ' ', ' '},
        {' ', ' ', ' ', '#', ' ', ' ', ' '},
        {' ', ' ', ' ', '#', ' ', ' ', ' '},
        {' ', ' ', ' ', '#', ' ', ' ', ' '},
        {' ', ' ', ' ', '#', ' ', ' ', ' '},
        {' ', ' ', ' ', '#', ' ', ' ', ' '},
        {' ', ' ', ' ', '#', ' ', ' ', ' '}}
};

static constexpr Config::digit_t eight{
       {{' ', ' ', '#', '#', '#', ' ', ' '},
        {' ', '#', ' ', ' ', ' ', '#', ' '},
        {' ', '#', ' ', ' ', ' ', '#', ' '},
        {' ', ' ', '#', '#', '#', ' ', ' '},
        {' ', '#', ' ', ' ', ' ', '#', ' '},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {' ', '#', ' ', ' ', ' ', '#', ' '},
        {' ', ' ', '#', '#', '#', ' ', ' '}}
};

static constexpr Config::digit_t nine{
       {{' ', ' ', '#', '#', '#', '#', ' '},
        {' ', '#', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', '#', '#'},
        {' ', '#', '#', '#', '#', ' ', '#'},
        {' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {' ', ' ', ' ', ' ', ' ', '#', ' '},
        {' ', '#', '#', '#', '#', ' ', ' '}}
};
};

    // clang-format on
public:
    Renderer() { clear_buffer(); }

    Config::digit_buffer_t get_buffer() const noexcept { return m_buffer; }

    void draw(std::size_t num)
    {
        _draw_impl(std::format(Config::DIGIT_FORMAT, num));
    }

    void draw(std::string_view num)
    {
        if (!std::ranges::all_of(num, isdigit))
            throw std::invalid_argument(
                std::format("{}: not an unsigned int",
                            std::source_location::current().function_name()));

        _draw_impl(std::format(Config::DIGIT_FORMAT, num));
    }

    void clear_buffer() noexcept
    {
        std::ranges::for_each(m_buffer,
                              [](auto& row) { std::ranges::fill(row, ' '); });
    }

    // draw buffer in console
    void display(std::ostream& stream = std::cout) const noexcept
    {
        std::ranges::for_each(m_buffer,
                              [&stream](const auto& row)
                              {
                                  std::ranges::copy(
                                      row, std::ostream_iterator<char>(stream));

                                  stream << '\n';
                              });
    }

private:
    Config::digit_buffer_t m_buffer;

    // fill map with defined digits
    inline static const std::unordered_map<char, Config::digit_t> numbers{
        {'0', char_digits::zero},  {'1', char_digits::one},
        {'2', char_digits::two},   {'3', char_digits::three},
        {'4', char_digits::four},  {'5', char_digits::five},
        {'6', char_digits::six},   {'7', char_digits::seven},
        {'8', char_digits::eight}, {'9', char_digits::nine}};

    void _draw_impl(std::string_view num) noexcept
    {
        _write_buffer(m_buffer, num);
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
                                std::size_t x_offset, std::size_t y_offset,
                                Config::digit_buffer_t& buffer) noexcept
    {
        for (size_t i = 0; i < Config::DIGIT_HEIGHT; ++i)
            for (size_t j = 0; j < Config::DIGIT_WIDTH; ++j)
                buffer[y_offset + i][x_offset + j] = digit[i][j];
    }
};

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: numberRenderer <number>\twhere <number> is "
                     "unsigned integer\n";
        return EXIT_FAILURE;
    }

    try
    {
        Renderer render;

        render.draw(argv[1]);
        render.display();
        render.clear_buffer();

        std::filesystem::path output_filename("numberRenderer_test_output.txt");
        if (std::ofstream out(output_filename); out.is_open())
        {
            render.draw(16101994UL);
            render.display(out);
            render.clear_buffer();
        }
        else
            std::cerr << std::format("{} was not open\n",
                                     output_filename.string());

#ifdef __linux
        std::filesystem::path null_file("/dev/null");
        if (std::ofstream out(null_file, std::ios::app); out.is_open())
        {
            render.draw(1488);
            render.display(out);
            render.clear_buffer();
        }
        else
            std::cerr << std::format("{} was not open\n", null_file.string());
#endif
    }
    catch (const std::exception& ex)
    {
        std::cerr << std::format("{}\n", ex.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
