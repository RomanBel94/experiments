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

    // Print current color.
    inline void _show_color(const std::string& color) const noexcept;

    // Common variables.
    static const std::string light_icon;
    static const std::unordered_map<std::string, const std::string> colors;

    // Configuration hashmap of this trafic light.
    mutable std::unordered_map<std::string, int> m_configuration;
};

const std::string TraficLightManager::light_icon{"***\n"};

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
    std::ifstream configuration_file("trafic_light_config.txt");
    if (!configuration_file)
        std::cerr
            << "Config file not open! Default delays (1sec) are applied.\n";
    else
    {
        std::clog << "[INFO] Loading";
        std::string parameter;
        while (configuration_file >> parameter)
        {
            configuration_file >> m_configuration[parameter];
            std::clog << ".";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::clog << '\n';
    }
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

        for (auto& config_element : m_configuration)
            _show_color(config_element.first);
    }
}

// Stops running this thread for given nuber of seconds.
void TraficLightManager::_sleep(int seconds) noexcept
{
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

// Prints current gonfiguration values.
void TraficLightManager::_print_current_configuration() const noexcept
{
    std::cout << "[INFO] Current delay parameters are: \n";
    for (const std::pair<std::string, int>& parameter : m_configuration)
    {
        std::cout << "[INFO] " << parameter.first << ": " << parameter.second
                  << " sec\n";
    }
}

// Shows current color on screen
void TraficLightManager::_show_color(const std::string& color) const noexcept
{
    std::cout << colors.at(color) << light_icon << colors.at("White");
    _sleep(m_configuration[color]);
}

int main()
{
    TraficLightManager trafic_light;
    trafic_light.run();

    return 0;
}
