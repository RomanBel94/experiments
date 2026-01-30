#include <cstddef>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <format>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

inline void recursive_print_directory(const fs::path& path)
{
    for (const auto& file : fs::recursive_directory_iterator(path))
    {
        std::cout << std::format("{}\n", file.path().string());
    }
}

int main(int argc, char** argv)
{
    fs::path current_dir;
    std::vector<fs::path> paths;

    if (argc == 1)
        current_dir = fs::current_path();
    else
        for (std::size_t i = 1; i < argc; ++i)
            paths.emplace_back(argv[i]);

    try
    {
        for (const auto& path : paths)
            recursive_print_directory(path);
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
