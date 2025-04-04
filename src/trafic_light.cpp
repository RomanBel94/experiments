#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>

class TraficLightManager
{
public:
    TraficLightManager();

    void run() noexcept;

private:
    inline void _print_current_parameters() noexcept;
    inline void _read_configuration_file() noexcept;

    std::ifstream configuration_file;
    std::unordered_map<std::string, int> delays;
};

TraficLightManager::TraficLightManager()
    : delays{{"Green", 1}, {"YellowToRed", 1}, {"Red", 1}, {"YellowToGreen", 1}}
{
    configuration_file.open("trafic_light_config.txt");
    if (!configuration_file)
        std::cerr << "Can't open configuration file! Default delay values are "
                     "applied.\n";
    else
    {
        _read_configuration_file();
    }
#ifndef DNDEBUG
    _print_current_parameters();
#endif
}

void TraficLightManager::run() noexcept
{
    std::cout << "Press Ctrl+C to stop the traffic light\n";
    while (true)
    {
        std::cout << "[\033[92m*\033[0m] Green " << delays["Green"]
                  << " sec...\n";
        std::this_thread::sleep_for(
            std::chrono::duration<int>(std::chrono::seconds(delays["Green"])));
        std::cout << "[\033[93m*\033[0m] " << delays["YellowToRed"]
                  << " sec...\n";
        std::this_thread::sleep_for(std::chrono::duration<int>(
            std::chrono::seconds(delays["YellowToRed"])));
        std::cout << "[\033[91m*\033[0m] Red " << delays["Red"] << " sec...\n";
        std::this_thread::sleep_for(
            std::chrono::duration<int>(std::chrono::seconds(delays["Red"])));
        std::cout << "[\033[93m*\033[0m] YellowToGreen "
                  << delays["YellowToGreen"] << " sec...\n";
        std::this_thread::sleep_for(std::chrono::duration<int>(
            std::chrono::seconds(delays["YellowToGreen"])));
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

void TraficLightManager::_read_configuration_file() noexcept
{
    std::string parameter;
    while (configuration_file >> parameter)
    {
        configuration_file >> delays[parameter];
    }
}

int main()
{
    TraficLightManager trafic_light;
    trafic_light.run();

    return 0;
}