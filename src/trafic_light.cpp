#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>

class TraficLightManager
{
public:
    // Constructor and destructor.
    TraficLightManager() noexcept;
    ~TraficLightManager() noexcept = default;

    // Main function of each trafic_light.
    void run() const noexcept;

private:
    // Delay switching to the next signal.
    static inline void _sleep(int seconds) noexcept;

    // Print debug information about current configuration.
    inline void _print_current_configuration() const noexcept;
    inline void _show_color(const std::string& color) const noexcept;

    // Common variables.
    static const std::string light_icon;
    static const std::string color_green;
    static const std::string color_yellow;
    static const std::string color_red;
    static const std::string color_white;

    static std::unordered_map<std::string, const std::string> colors;

    // Configuration hashmap of this trafic light.
    mutable std::unordered_map<std::string, int> m_configuration;
};

// Definition of common variables.
const std::string TraficLightManager::light_icon(3, 219U);
const std::string TraficLightManager::color_green{"\033[92m"};
const std::string TraficLightManager::color_yellow{"\033[93m"};
const std::string TraficLightManager::color_red{"\033[91m"};
const std::string TraficLightManager::color_white{"\033[0m\n"};

std::unordered_map<std::string, const std::string> TraficLightManager::colors{
    {"Green", TraficLightManager::color_green},
    {"YellowToRed", TraficLightManager::color_yellow},
    {"Red", TraficLightManager::color_red},
    {"YellowToGreen", TraficLightManager::color_yellow}};

// Reads configuration file and fills self configuration hashmap,
// if can't read configuration file applies default values.
TraficLightManager::TraficLightManager() noexcept
    : m_configuration{
          {"Green", 1}, {"YellowToRed", 1}, {"Red", 1}, {"YellowToGreen", 1}}
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
            configuration_file >> m_configuration[parameter];
        }
    }
#ifndef NDEBUG
    _print_current_configuration();
#endif
}

// Shows current color on screen
void TraficLightManager::_show_color(const std::string& color) const noexcept
{
    std::cout << colors[color] << light_icon << color_white;
    _sleep(m_configuration[color]);
}

// Endless loop that prints current light icon with different colors.
void TraficLightManager::run() const noexcept
{
    size_t counter{0};
    while (true)
    {
        if (counter++ % 5 == 0)
            std::cout << "Press Ctrl+C to stop the traffic light\n";

        _show_color("Green");
        _show_color("YellowToRed");
        _show_color("Red");
        _show_color("YellowToGreen");
    }
}

#ifndef NDEBUG
// Prints current gonfiguration values.
void TraficLightManager::_print_current_configuration() const noexcept
{
    std::cout << "[DEBUG] Current delay parameters are: \n";
    for (const std::pair<std::string, int>& parameter : m_configuration)
    {
        std::cout << "[DEBUG] " << parameter.first << ": " << parameter.second
                  << " sec\n";
    }
}
#endif

// Stops running this thread for given nuber of seconds.
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
