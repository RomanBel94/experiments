#include <algorithm>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <format>
#include <iostream>
#include <ranges>
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
    std::vector<fs::path> paths;

    if (argc == 1)
        paths.emplace_back(fs::current_path());
    else
        for (auto i : std::views::iota(1, argc))
            paths.emplace_back(argv[i]);

    try
    {
        std::ranges::for_each(paths, recursive_print_directory);
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
