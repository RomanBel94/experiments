#include <algorithm>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <format>
#include <iostream>
#include <ranges>
#include <vector>

namespace fs = std::filesystem;
namespace rng = std::ranges;
namespace views = std::views;

inline void recursive_print_directory(const fs::path& path)
{
    rng::for_each(fs::recursive_directory_iterator(path), [](const auto& entry)
                  { std::cout << std::format("{}\n", entry.path().string()); });
}

int main(int argc, char** argv)
{
    std::vector<fs::path> paths;

    if (argc == 1)
        paths.emplace_back(fs::current_path());
    else
        rng::for_each(views::iota(1, argc),
                      [&paths, argv](int i) { paths.emplace_back(argv[i]); });

    try
    {
        rng::for_each(paths, recursive_print_directory);
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
