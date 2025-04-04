#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>

class TraficLightManager
{
public:
    TraficLightManager() noexcept;
    ~TraficLightManager() noexcept = default;

    void run() noexcept;

private:
    static inline void _sleep(int seconds) noexcept;

    inline void _print_current_parameters() noexcept;

    static const std::string light_icon;
    static const std::string color_green;
    static const std::string color_yellow;
    static const std::string color_red;
    static const std::string color_white;

    std::unordered_map<std::string, int> delays;
};

const std::string TraficLightManager::light_icon(3, 219U);
const std::string TraficLightManager::color_green{"\033[92m"};
const std::string TraficLightManager::color_yellow{"\033[93m"};
const std::string TraficLightManager::color_red{"\033[91m"};
const std::string TraficLightManager::color_white{"\033[0m\n"};

TraficLightManager::TraficLightManager() noexcept
    : delays{{"Green", 1}, {"YellowToRed", 1}, {"Red", 1}, {"YellowToGreen", 1}}
{
    std::ifstream configuration_file("trafic_light_config.txt");
    if (!configuration_file)
        std::cerr
            << "Config file not open! Default delays (1sec) are applied.\n";
    else
    {
        std::string parameter;
        while (configuration_file >> parameter)
        {
            configuration_file >> delays[parameter];
        }
    }
#ifndef DNDEBUG
    _print_current_parameters();
#endif
}

void TraficLightManager::run() noexcept
{
    size_t counter{0};
    while (true)
    {
        if (counter++ % 5 == 0)
            std::cout << "Press Ctrl+C to stop the traffic light\n";

        std::cout << color_green << light_icon << color_white;
        _sleep(delays["Green"]);

        std::cout << color_yellow << light_icon << color_white;
        _sleep(delays["YellowToRed"]);

        std::cout << color_red << light_icon << color_white;
        _sleep(delays["Red"]);

        std::cout << color_yellow << light_icon << color_white;
        _sleep(delays["YellowToGreen"]);
    }
}

void TraficLightManager::_print_current_parameters() noexcept
{
    std::cout << "[DEBUG] Current delay parameters are: \n";
    for (const std::pair<std::string, int>& delay : delays)
    {
        std::cout << "[DEBUG] " << delay.first << ' ' << delay.second << '\n';
    }
}

void TraficLightManager::_sleep(int seconds) noexcept
{
    std::this_thread::sleep_for(
        std::chrono::duration<int>(std::chrono::seconds(seconds)));
}

int main()
{
    TraficLightManager trafic_light;
    trafic_light.run();

    return 0;
}