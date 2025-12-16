#include <cwctype>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

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
        return std::format(L"Value: {: <35}line: {:<10}pos: {:<10}",
                           (value == L"\n" ? L"\\n" : value), line, pos);
    }

    bool operator==(const std::wstring& rhs) const noexcept
    {
        return value == rhs;
    }

    mutable std::wstring value;
    const std::size_t line;
    const std::size_t pos;
};

class CT_Lexer
{
public:
    void get_tokens(const std::filesystem::path& filepath);
    void clear() { tokens.clear(); }

    const std::list<Token>& get_tokens() const noexcept { return tokens; }

private:
    struct LexerContext
    {
        std::size_t current_line{1};
        std::size_t current_pos{1};

        std::wifstream input_file;

        void open_file(const std::filesystem::path& filepath)
        {
            input_file.open(filepath);
            if (!input_file)
                throw std::runtime_error("[FATAL] Can't open input file!");
        }

        wchar_t peek() { return input_file.peek(); }
        wchar_t get()
        {
            ++current_pos;
            return input_file.get();
        }
        bool eof() const noexcept { return input_file.eof(); }

    } context;

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
    bool _is_comment(const wchar_t ch) const noexcept { return ch == L'.'; }
};

void CT_Lexer::get_tokens(const std::filesystem::path& filepath)
{
    context.open_file(filepath);
    std::wstring temp;

    while (!context.eof())
    {
        while (_is_wspace(context.peek())) // skip spaces
        {
            context.get();
        }
        if (_is_comment(context.peek())) // skip comments
        {
            while (!_is_wnewline(context.peek()))
                context.get();
        }
        if (_is_wnewline(context.peek())) // found new line
        {
            context.get();
            ++context.current_line;
            context.current_pos = 1;
            continue;
        }
        else if (_is_wquot(context.peek())) // read quoted text
        {
            temp += context.get();
            while (!_is_wquot(context.peek()))
            {
                temp += context.get();
            }
            temp += context.get();
        }
        else if (_is_wtext(context.peek())) // read word
        {
            while (_is_wtext(context.peek()))
            {
                temp += context.get();
                ++context.current_pos;
            }
        }

        tokens.emplace_back(temp, context.current_line, context.current_pos);
        temp.clear();
    }

    // remove checksums
    tokens.erase(std::find(tokens.cbegin(), tokens.cend(), L"Checksum_A:"),
                 tokens.end());
}

class CommandTableParser final
{
    using header_t = std::unordered_map<std::string, std::string>;

public:
    CommandTableParser(const std::filesystem::path& input);

    void parse();

private:
    std::filesystem::path m_command_table_path;

    CT_Lexer m_lexer;

    header_t m_header{};
    // cos_interface_t m_cos_interface
    // components_t m_component
    // commands_t m_commands
};

CommandTableParser::CommandTableParser(const std::filesystem::path& input)
    : m_command_table_path(input), m_header() {};

void CommandTableParser::parse() { m_lexer.get_tokens(m_command_table_path); }

int main()
{
    std::filesystem::path command_table_input_filename = "Command_table";
    std::filesystem::path command_table_output_filename =
        "Command_table_output";

    CT_Lexer lexer;
    try
    {
        using namespace std::chrono;
        auto start = high_resolution_clock::now();

        std::clog << "Start extracting tokens\n";
        lexer.get_tokens(command_table_input_filename);

        duration<double> time_passed = high_resolution_clock::now() - start;

        std::clog << std::format("Tokens extracted. Time passed {:.5f}s\n",
                                 time_passed.count());
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what();
        return EXIT_FAILURE;
    }

    std::clog << "Start writing tokens\n";

    for (std::wofstream ct_out(command_table_output_filename);
         const auto& token : lexer.get_tokens())
    {
        ct_out << token.to_wstring() << L'\n';
    }

    constexpr std::string_view crc_command =
        "X:\\eqv\\gen\\CRC\\CRC-3.2.2\\CRC\\distribution\\bin\\WinXP\\crc.exe "
        "{:s}";
    std::system(std::format(crc_command, command_table_output_filename.string())
                    .c_str());

    return EXIT_SUCCESS;
}
