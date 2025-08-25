#include <cwctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include <unordered_map>

class CommandTable final
{
    using header_t = std::list<std::pair<const std::string, const std::string>>;

public:
    CommandTable() = default;

    header_t& get_header() { return header; }

private:
    header_t header{};
};

struct Token
{
    Token(const std::wstring& value, std::size_t line = 1, std::size_t pos = 1)
        : value{value}, line{line}, pos{pos - value.size()}
    {
    }

    Token(wchar_t ch, std::size_t line = 1, std::size_t pos = 1)
        : value{1, ch}, line{line}, pos{pos - 1}
    {
    }

    std::wstring to_wstring() const
    {
        return L"Value: " + value + L" line: " + std::to_wstring(line) +
               L" pos: " + std::to_wstring(pos);
    }

    const std::wstring value;
    const std::size_t line;
    const std::size_t pos;
};

class CT_Lexer
{
public:
    void extract_tokens(const std::string& filepath);
    const std::list<Token> get_tokens() const noexcept { return tokens; }

private:
    std::size_t current_line{1};
    std::size_t current_pos{1};

    std::list<Token> tokens;

    bool _iswspace(const wchar_t ch) const noexcept
    {
        return std::iswspace(ch) && !_iswnewline(ch);
    }
    bool _iswtext(const wchar_t ch) const noexcept
    {
        return !_iswspace(ch) && !_iswnewline(ch) && std::iswprint(ch);
    }
    bool _iswquot(const wchar_t ch) const noexcept { return ch == L'\"'; }
    bool _iswnewline(const wchar_t ch) const noexcept { return ch == L'\n'; }
    bool _iscomment(const wchar_t ch) const noexcept
    {
        return ch == L'.' && current_pos == 1;
    }
};

void CT_Lexer::extract_tokens(const std::string& filepath)
{
    std::wifstream input_file{filepath};
    if (!input_file)
        throw std::runtime_error{"[FATAL] Can't open input file: " + filepath};

    std::wstring temp;

    while (!input_file.eof())
    {
        if (input_file.eof())
            break;

        temp.clear();
        while (_iswspace(input_file.peek()))
        {
            input_file.get();
            ++current_pos;
        }
        if (_iswnewline(input_file.peek()))
        {
            temp += static_cast<wchar_t>(input_file.get());
            ++current_line;
            current_pos = 1;
        }
        else if (_iscomment(input_file.peek()))
        {
            while (!_iswnewline(input_file.peek()))
                input_file.get();
        }
        else if (_iswquot(input_file.peek()))
        {
            temp += static_cast<wchar_t>(input_file.get());
            while (!_iswquot(input_file.peek()))
            {
                temp += static_cast<wchar_t>(input_file.get());
            }
            temp += static_cast<wchar_t>(input_file.get());
        }
        else if (_iswtext(input_file.peek()))
        {
            while (_iswtext(input_file.peek()))
            {
                temp += static_cast<wchar_t>(input_file.get());
                ++current_pos;
            }
        }

        tokens.emplace_back(std::move(temp), current_line, current_pos);
    }

    tokens.emplace_back(L"EOF");
    std::wcout << L"Tokens were collected\n";
    // for(const auto& token : tokens)
    //{
    //     std::wcout << token.to_wstring() << L"\n";
    // }
}

int main()
{
    CT_Lexer lexer;
    try
    {
        lexer.extract_tokens("Command_table");
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}