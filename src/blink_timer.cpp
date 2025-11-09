#include <chrono>
#include <cmath>
#include <iostream>

class blink_timer final
{
private:
    blink_timer() = delete;
    blink_timer(const blink_timer&) = delete;
    blink_timer(blink_timer&&) noexcept = delete;
    blink_timer& operator=(const blink_timer&) = delete;
    blink_timer&& operator=(blink_timer&&) noexcept = delete;

    std::chrono::duration<double> active_phase{};
    std::chrono::duration<double> not_active_phase{};
    std::chrono::duration<double> full_cycle{};

    bool is_active{false};

    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

public:
    blink_timer(double not_active_phase, double active_phase)
        : active_phase(active_phase), not_active_phase(not_active_phase),
          full_cycle(active_phase + not_active_phase)
    {
        start_time = std::chrono::high_resolution_clock::now();
    }

    ~blink_timer() noexcept = default;

    bool is_active_phase() const noexcept
    {
        auto current_time = std::chrono::high_resolution_clock::now();
        auto time_passed = current_time - start_time;
        auto num_cycles = std::floor(time_passed / full_cycle);

        return ((time_passed - num_cycles * full_cycle) < active_phase);
    }
};

int main()
{
    blink_timer timer(.5, .5);
    while (true)
    {
        if (timer.is_active_phase())
        {
            std::cout << "active\n";
            continue;
        }
        std::cout << "not active\n";
    }
}
