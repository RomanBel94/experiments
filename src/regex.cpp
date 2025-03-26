#include <iostream>
#include <regex>
#include <string>

char show_menu(std::regex& regex, std::string& regex_string);
void update_regex(std::regex& regex, std::string& regex_string);
void check_matching(const std::regex& regex, const std::string& regex_string);

int main()
{
    static std::string regex_string;
    static std::regex regex;
    char choice;

    do
    {
        choice = show_menu(regex, regex_string);
        switch (choice)
        {
        case '1':
            update_regex(regex, regex_string);
            break;
        case '2':
            check_matching(regex, regex_string);
            break;
        case 'Q':
            break;
        default:
            std::cout << "Invalid choice, try again!";
        }
    } while (choice != 'Q');

    return EXIT_SUCCESS;
}

char show_menu(std::regex& regex, std::string& regex_string)
{
    std::cout << "\n\n---*** MENU ***---\n\n"
              << "1 - Update regex template\n"
              << "2 - Enter string and check matching to current regex\n"
              << "Q - Quit\n\n"
              << "Current regex template: " << regex_string << "\n\n"
              << "Enter your choice: ";
    char choice;
    std::cin >> choice;
    return choice;
}

void update_regex(std::regex& regex, std::string& regex_string)
{
    std::cout << "Enter new regex: ";
    std::getline(std::cin, regex_string);
    std::getline(std::cin, regex_string);
    regex = regex_string;
}

void check_matching(const std::regex& regex, const std::string& regex_string)
{
    std::string check_string{""};
    std::cout << "Enter string to check: ";
    std::getline(std::cin, check_string);
    std::getline(std::cin, check_string);

    std::cout << check_string;
    if (std::regex_match(check_string, regex))
    {
        std::cout << " matches with current regex\n";
    }
    else
    {
        std::cout << " doesn't match with current regex\n";
    }
}