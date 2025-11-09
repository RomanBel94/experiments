#include <chrono>
#include <cstddef>
#include <iostream>
#include <ratio>

template <std::size_t pos_phase, std::size_t neg_phase>
class blink_timer final
{
private:
    blink_timer(const blink_timer&) = delete;
    blink_timer(blink_timer&&) noexcept = delete;
    blink_timer& operator=(const blink_timer&) = delete;
    blink_timer&& operator=(blink_timer&&) noexcept = delete;

    const std::chrono::duration<std::size_t, std::milli> _positive_phase{
        pos_phase};
    const std::chrono::duration<std::size_t, std::milli> _negative_phase{
        neg_phase};

    const std::chrono::time_point<std::chrono::high_resolution_clock>
        start_time{};

public:
    blink_timer() : start_time(std::chrono::high_resolution_clock::now()) {}

    ~blink_timer() noexcept = default;

    bool current_phase() noexcept
    {
        auto current_time = std::chrono::high_resolution_clock::now();
        auto time_passed = current_time - start_time;
        auto full_cycle = _positive_phase + _negative_phase;
        std::size_t num_cycles = time_passed / full_cycle;

        return ((time_passed - num_cycles * full_cycle) < _positive_phase);
    }
};

int main()
{
    blink_timer<500, 500> timer;

    std::cout << std::boolalpha;
    while (true)
        std::cout << timer.current_phase() << '\n';
}
