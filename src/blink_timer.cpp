#include <chrono>
#include <cstddef>
#include <iostream>
#include <ratio>

class blink_timer final
{
private:
    blink_timer() = delete;
    blink_timer(const blink_timer&) = delete;
    blink_timer(blink_timer&&) noexcept = delete;
    blink_timer& operator=(const blink_timer&) = delete;
    blink_timer&& operator=(blink_timer&&) noexcept = delete;

    std::chrono::duration<std::size_t, std::milli> _positive_phase{};
    std::chrono::duration<std::size_t, std::milli> _negative_phase{};
    std::chrono::duration<std::size_t, std::milli> _full_cycle{};

    bool _is_active{false};

    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

public:
    blink_timer(std::size_t positive_phase, std::size_t negative_phase)
        : _positive_phase(positive_phase), _negative_phase(negative_phase),
          _full_cycle(_positive_phase + _negative_phase)
    {
        start_time = std::chrono::high_resolution_clock::now();
    }

    ~blink_timer() noexcept = default;

    bool current_phase() noexcept
    {
        auto current_time = std::chrono::high_resolution_clock::now();
        auto time_passed = current_time - start_time;
        std::size_t num_cycles = time_passed / _full_cycle;

        return ((time_passed - num_cycles * _full_cycle) < _positive_phase);
        ;
    }
};

int main()
{
    blink_timer timer(500, 500);

    while (true)
    {
        std::cout << std::boolalpha << timer.current_phase() << '\n';
    }
}
