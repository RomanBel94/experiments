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
#include <format>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string_view>
#include <unordered_map>

namespace rng = std::ranges;
namespace views = std::views;

class Renderer final
{
private:
    struct Config
    {
        static constexpr size_t VERTICAL_SPACE = 1;
        static constexpr size_t HORIZONTAL_SPACE = 1;
        static constexpr size_t DIGIT_NUMBER = 12;
        static constexpr auto DIGIT_FORMAT = "{:0>{}}";
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
    ~Renderer() = default;

    void draw(std::size_t num)
    {
        _draw_impl(
            std::format(Config::DIGIT_FORMAT, num, Config::DIGIT_NUMBER));
    }

    void draw(std::string_view num)
    {
        assert(rng::all_of(num, isdigit));
        _draw_impl(
            std::format(Config::DIGIT_FORMAT, num, Config::DIGIT_NUMBER));
    }

    void clear_buffer() noexcept
    {
        rng::for_each(m_buffer, [](auto& row) { row.fill(' '); });
    }

    // draw buffer in console
    void display(std::ostream& stream = std::cout) const noexcept
    {
        rng::for_each(m_buffer,
                      [&stream](const auto& row)
                      {
                          rng::copy(row, std::ostream_iterator<char>(stream));
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

    void _draw_impl(std::string_view num) noexcept { _write_buffer(num); }

    void _write_buffer(std::string_view num)
    {
        rng::for_each(views::iota(0ul, Config::DIGIT_NUMBER),
                      [&](auto i)
                      {
                          _write_digit_to_buffer(
                              numbers.at(num[i]),
                              (Config::DIGIT_WIDTH + Config::HORIZONTAL_SPACE) *
                                      i +
                                  Config::HORIZONTAL_SPACE,
                              Config::VERTICAL_SPACE);
                      });
    }

    void _write_digit_to_buffer(Config::digit_t const& digit, size_t x_offset,
                                size_t y_offset) noexcept
    {
        for (auto i : views::iota(0ul, Config::DIGIT_HEIGHT))
            for (auto j : views::iota(0ul, Config::DIGIT_WIDTH))
                m_buffer[y_offset + i][x_offset + j] = digit[i][j];
    }
};

std::ostream& operator<<(std::ostream& stream, const Renderer& render)
{
    render.display(stream);
    return stream;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: numberRenderer <number>\twhere <number> is "
                     "unsigned integer\n";
        return EXIT_FAILURE;
    }

    Renderer render;

    render.draw(argv[1]);
    render.display();

    return EXIT_SUCCESS;
}
