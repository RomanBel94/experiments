#include <cstdlib>
#include <filesystem>
#include <format>
#include <iostream>

namespace fs = std::filesystem;

int main(int, char**)
{
    fs::path current_dir = fs::current_path();

    for (const auto& file : fs::recursive_directory_iterator(current_dir))
    {
        std::cout << std::format("{}\n", file.path().string());
    }

    return EXIT_SUCCESS;
}
