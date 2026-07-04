#include <chrono>
#include <cstddef>
#include <iostream>

template <size_t _pos_phase, size_t _neg_phase,
          class _clock = std::chrono::high_resolution_clock>
class blink_timer final
{
private:
    blink_timer(const blink_timer&) = delete;
    blink_timer(blink_timer&&) noexcept = delete;
    blink_timer& operator=(const blink_timer&) = delete;
    blink_timer&& operator=(blink_timer&&) noexcept = delete;

    const std::chrono::milliseconds _positive_phase{_pos_phase};
    const std::chrono::milliseconds _negative_phase{_neg_phase};

    const std::chrono::time_point<_clock> start_time;

public:
    blink_timer() : start_time(_clock::now()){};

    bool current_phase() const noexcept
    {
        auto time_passed = _clock::now() - start_time;
        auto full_cycle = _positive_phase + _negative_phase;
        std::size_t num_cycles = time_passed / full_cycle;

        return ((time_passed - num_cycles * full_cycle) < _positive_phase);
    }
};

int main()
{
    blink_timer<1000, 500> timer;

    std::boolalpha(std::cout);
    while (true)
        std::cout << timer.current_phase() << '\n';
}
