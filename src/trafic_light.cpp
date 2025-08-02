#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>

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
    static inline void _sleep(const int seconds) noexcept;

    // Print debug information about current configuration.
    inline void _print_current_configuration() const noexcept;

    // Reads config file and fills configuration map.
    inline void _read_configuration(std::ifstream& config_file) const noexcept;

    // Print current color.
    inline void _show_color(const std::string& color) const noexcept;

    // Common variables.
    static const std::string light_icon;
    static const std::unordered_map<std::string, const std::string> colors;

    // Configuration hashmap of this trafic light.
    mutable std::unordered_map<std::string, int> m_configuration;
};

std::string operator""_s(char const* const str, size_t size)
{
    return std::string(str);
}

const std::string TraficLightManager::light_icon{"***\n"_s};

const std::unordered_map<std::string, const std::string>
    TraficLightManager::colors{
        {"Green"_s, "\033[92m"_s}, {"YellowToRed"_s, "\033[93m"_s},
        {"Red"_s, "\033[91m"_s},   {"YellowToGreen"_s, "\033[93m"_s},
        {"White"_s, "\033[0m"_s},
    };

// Reads configuration file and fills self configuration hashmap,
// if can't read configuration file applies default values.
TraficLightManager::TraficLightManager() noexcept
    : m_configuration{{"Green"_s, 1},
                      {"YellowToRed"_s, 1},
                      {"Red"_s, 1},
                      {"YellowToGreen"_s, 1}}
{
    std::ifstream configuration_file("trafic_light_config.txt");
    if (!configuration_file)
        std::cerr
            << "Config file not open! Default delays (1sec) are applied.\n";
    else
        _read_configuration(configuration_file);

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

        std::for_each(
            m_configuration.begin(), m_configuration.end(),
            [this](const std::pair<const std::string, int>& config_element)
            { this->_show_color(config_element.first); });
    }
}

// Stops running this thread for given nuber of seconds.
void TraficLightManager::_sleep(const int seconds) noexcept
{
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

// Prints current gonfiguration values.
void TraficLightManager::_print_current_configuration() const noexcept
{
    std::cout << "[INFO] Current delay parameters are: \n";

    std::for_each(
        m_configuration.begin(), m_configuration.end(),
        [this](const std::pair<const std::string, int>& config_element)
        {
            std::cout << "[INFO] " << config_element.first << ": "
                      << config_element.second << " sec\n";
        });
}

// Reads config file and fills configuration map.
inline void TraficLightManager::_read_configuration(
    std::ifstream& config_file) const noexcept
{
    std::clog << "[INFO] Loading";
    std::string parameter;

    while (config_file >> parameter)
    {
        config_file >> m_configuration[parameter];
        std::clog << ".";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::clog << '\n';
}

// Shows current color on screen
void TraficLightManager::_show_color(const std::string& color) const noexcept
{
    std::cout << colors.at(color) << light_icon << colors.at("White"_s);
    _sleep(m_configuration.at(color));
}

int main()
{
    const TraficLightManager trafic_light;
    trafic_light.run();

    return 0;
}
