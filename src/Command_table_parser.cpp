#include <cwctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <sstream>
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

    std::wstring to_wstring() const noexcept
    {
        std::wostringstream string;
        string << L"Value: " << std::setw(35) << std::left
               << (value == L"\n" ? L"\\n" : value) << L" line: "
               << std::setw(8) << std::left << line << L" pos: " << std::setw(8)
               << std::left << pos;
        return string.str();
    }

    bool operator==(const std::wstring& rhs) const noexcept
    {
        return value == rhs;
    }
    bool operator==(const wchar_t* const rhs) const noexcept
    {
        return value == rhs;
    }
    const Token& operator=(const std::wstring& rhs) noexcept
    {
        value = rhs;
        return *this;
    }
    const Token& operator=(const wchar_t* const rhs) noexcept
    {
        value = rhs;
        return *this;
    }
    const Token& operator=(const Token& rhs) noexcept
    {
        value = rhs.value;
        return *this;
    }
    const Token operator+(const Token& rhs) const noexcept
    {
        return Token(value + L' ' + rhs.value, line, pos);
    }
    void operator+=(const Token& rhs) const noexcept
    {
        value = value + L' ' + rhs.value;
    }

    mutable std::wstring value;
    const std::size_t line;
    const std::size_t pos;
};

class CT_Lexer
{
public:
    void extract_tokens(const std::string& filepath);
    void clear() { tokens.clear(); }

    const std::list<Token>& get_tokens() const noexcept { return tokens; }

private:
    std::size_t current_line{1};
    std::size_t current_pos{1};

    std::list<Token> tokens;

    bool _is_wspace(const wchar_t ch) const noexcept
    {
        return std::iswspace(ch) && !_is_wnewline(ch);
    }
    bool _is_wtext(const wchar_t ch) const noexcept
    {
        return !_is_wspace(ch) && !_is_wnewline(ch) && std::iswprint(ch);
    }
    bool _is_wquot(const wchar_t ch) const noexcept { return ch == L'\"'; }
    bool _is_wnewline(const wchar_t ch) const noexcept { return ch == L'\n'; }
    bool _is_comment(const wchar_t ch) const noexcept
    {
        return ch == L'.' && current_pos == 1;
    }
    void _compress_tokens() noexcept;
};

void CT_Lexer::extract_tokens(const std::string& filepath)
{
    std::wifstream input_file{filepath};
    if (!input_file)
        throw std::runtime_error{"[FATAL] Can't open input file: " + filepath};

    std::wstring temp;

    while (!input_file.eof())
    {
        while (_is_wspace(input_file.peek()))
        {
            input_file.get();
            ++current_pos;
        }
        if (_is_comment(input_file.peek()))
        {
            while (!_is_wnewline(input_file.peek()))
                input_file.get();
        }
        if (_is_wnewline(input_file.peek()))
        {
            input_file.get();
            ++current_line;
            current_pos = 1;
            continue;
        }
        else if (_is_wquot(input_file.peek()))
        {
            temp += static_cast<wchar_t>(input_file.get());
            while (!_is_wquot(input_file.peek()))
            {
                temp += static_cast<wchar_t>(input_file.get());
            }
            temp += static_cast<wchar_t>(input_file.get());
        }
        else if (_is_wtext(input_file.peek()))
        {
            while (_is_wtext(input_file.peek()))
            {
                temp += static_cast<wchar_t>(input_file.get());
                ++current_pos;
            }
        }

        tokens.emplace_back(std::move(temp), current_line, current_pos);
        temp.clear();
    }

    tokens.emplace_back(L"EOF");
    _compress_tokens();
}

void CT_Lexer::_compress_tokens() noexcept
{
    for (auto current_token = tokens.begin(); current_token != tokens.end();
         ++current_token)
    {
        if (*current_token == L"COS" || *current_token == L"Main")
        {
            auto next_token = current_token;
            ++next_token;
            *current_token += *next_token;
            tokens.erase(next_token);
        }
        else if (*current_token == L"End")
        {
            auto next_token = current_token;
            ++next_token;
            for (std::size_t cnt{1}; cnt < 4; ++cnt)
            {
                *current_token += *next_token;
                tokens.erase(next_token++);
            }
        }
        else if (*current_token == L"")
        {
            tokens.erase(current_token++);
        }
    }
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
    for (const auto& token : lexer.get_tokens())
        std::wcout << token.to_wstring() << L'\n';

    return EXIT_SUCCESS;
}
