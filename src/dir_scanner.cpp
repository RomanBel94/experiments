#include <cstdlib>
#include <exception>
#include <filesystem>
#include <format>
#include <iostream>

namespace fs = std::filesystem;

int main(int argc, char** argv)
{
    fs::path current_dir;

    if (argc == 1)
        current_dir = fs::current_path();
    else
        current_dir = argv[1];

    try
    {
        for (const auto& file : fs::recursive_directory_iterator(current_dir))
        {
            std::cout << std::format("{}\n", file.path().string());
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
