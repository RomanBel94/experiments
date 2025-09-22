#include <iostream>
#include <regex>
#include <string>

char show_menu(std::string& regex_string, bool last_match);
void update_regex(std::regex& regex, std::string& regex_string);
void check_matching(const std::regex& regex, const std::string& regex_string,
                    bool& match_result);

std::ostream& red_color(std::ostream& stream);
std::ostream& green_color(std::ostream& stream);
std::ostream& white_color(std::ostream& stream);

int main()
{
    std::string regex_string{
        R"([\d|\w]+(\.[\d|\w]+)*@[\d|\w]+(\.[\d|\w]+)*\.\w+)"};
    std::regex regex{regex_string};
    char choice{};
    bool last_match{};

    do
    {
        choice = show_menu(regex_string, last_match);
        switch (choice)
        {
        case '1':
            update_regex(regex, regex_string);
            break;
        case '2':
            check_matching(regex, regex_string, last_match);
            break;
        case 'Q':
            break;
        default:
            std::cout << "Invalid choice, try again!";
        }
    } while (choice != 'Q');

    return EXIT_SUCCESS;
}

char show_menu(std::string& regex_string, bool last_match)
{
    std::cout << "\n\n---*** MENU ***---\n\n"
              << "1 - Update regex template\n"
              << "2 - Enter string and check matching to current regex\n"
              << "Q - Quit\n\n"
              << "Current regex template: " << regex_string << "\n"
              << "Last match result: " << std::boolalpha;

    if (last_match)
        std::cout << green_color;
    else
        std::cout << red_color;

    std::cout << last_match << white_color << "\n\nEnter your choice: ";

    char choice;
    std::cin >> choice;
    return choice;
}

void update_regex(std::regex& regex, std::string& regex_string)
{
    std::cout << "\n\nEnter new regex: ";
    std::getline(std::cin, regex_string);
    std::getline(std::cin, regex_string);
    regex = regex_string;
}

void check_matching(const std::regex& regex, const std::string& regex_string,
                    bool& match_result)
{
    std::string check_string{""};
    std::cout << "Enter string to check: ";
    std::getline(std::cin, check_string);
    std::getline(std::cin, check_string);

    std::cout << check_string;
    match_result = std::regex_match(check_string, regex);
}

std::ostream& red_color(std::ostream& stream) { return stream << "\033[91m"; }

std::ostream& green_color(std::ostream& stream) { return stream << "\033[92m"; }

std::ostream& white_color(std::ostream& stream) { return stream << "\033[0m"; }
