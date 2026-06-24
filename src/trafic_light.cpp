#include <algorithm>
#include <chrono>
#include <format>
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
    // Other constructors and assignment operators are deleted.
    TraficLightManager(const TraficLightManager&) = delete;
    TraficLightManager(TraficLightManager&&) noexcept = delete;
    TraficLightManager& operator=(const TraficLightManager&) = delete;
    TraficLightManager& operator=(TraficLightManager&&) noexcept = delete;

    // Delay switching to the next signal.
    static void _sleep(const int seconds) noexcept;

    // Print debug information about current configuration.
    void _print_current_configuration() const noexcept;

    // Reads config file and fills configuration map.
    void _read_configuration(std::ifstream& config_file) const;

    // Print current color.
    void _show_color(const std::string& color) const noexcept;

    // Common variables.
    inline static const std::string light_icon{"***"};
    static const std::unordered_map<std::string, const std::string> colors;

    // Configuration hashmap of this trafic light.
    mutable std::unordered_map<std::string, int> m_configuration;
};

const std::unordered_map<std::string, const std::string>
    TraficLightManager::colors{
        {"Green", "\033[92m"}, {"YellowToRed", "\033[93m"},
        {"Red", "\033[91m"},   {"YellowToGreen", "\033[93m"},
        {"White", "\033[0m"},
    };

// Reads configuration file and fills self configuration hashmap,
// if can't read configuration file applies default values.
TraficLightManager::TraficLightManager() noexcept
    : m_configuration{
          {"Green", 1}, {"YellowToRed", 1}, {"Red", 1}, {"YellowToGreen", 1}}
{

    if (std::ifstream configuration_file("trafic_light_config.txt");
        configuration_file)
        _read_configuration(configuration_file);
    else
        std::cerr
            << "Config file not open! Default delays (1sec) are applied.\n";

    _print_current_configuration();
}

// Endless loop that prints current light icon with different colors.
void TraficLightManager::run() const noexcept
{
    size_t counter{0};
    while (true)
    {
        if (counter++ % 5 == 0)
            std::cout << "[INFO] Press Ctrl+C to stop the traffic light\n";

        std::ranges::for_each(
            m_configuration,
            [this](const std::pair<const std::string, int>& config_element)
            { this->_show_color(config_element.first); });
    }
}

// Pauses running this thread for given nuber of seconds.
void TraficLightManager::_sleep(const int seconds) noexcept
{
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

// Prints current gonfiguration values.
void TraficLightManager::_print_current_configuration() const noexcept
{
    std::cout << "[INFO] Current delay parameters are: \n";

    std::ranges::for_each(
        m_configuration,
        [](const std::pair<const std::string, int>& config_element)
        {
            std::cout << std::format("[INFO] {:<15} : {}sec\n",
                                     config_element.first,
                                     config_element.second);
        });
}

// Reads config file and fills configuration map.
void TraficLightManager::_read_configuration(std::ifstream& config_file) const
{
    std::clog << "[INFO] Loading config\n";
    std::string parameter;

    while (config_file >> parameter)
    {
        config_file >> m_configuration[parameter];
    }
}

// Shows current color on screen
void TraficLightManager::_show_color(const std::string& color) const noexcept
{
    std::cout << std::format("{}{}{}\n", colors.at(color), light_icon,
                             colors.at("White"));
    _sleep(m_configuration.at(color));
}

int main()
{
    const TraficLightManager trafic_light;
    trafic_light.run();

    return 0;
}
