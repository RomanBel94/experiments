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
    Token(const std::string& value, std::size_t line = 1, std::size_t pos = 1)
        : value{value}, line{line}, pos{pos - value.size()}
    {
    }

    Token(char ch, std::size_t line = 1, std::size_t pos = 1)
        : value{1, ch}, line{line}, pos{pos - 1}
    {
    }

    std::string to_string() const noexcept
    {
        return std::format("Value: {: <35}line: {:<10}pos: {:<10}",
                           (value == "\n" ? "\\n" : value), line, pos);
    }

    auto operator<=>(const Token&) const = default;

    bool operator==(const std::string& rhs) const noexcept
    {
        return value == rhs;
    }

    const std::string value;
    const std::size_t line;
    const std::size_t pos;
};

class CommandTableLexer
{
public:
    void extract_tokens(const std::filesystem::path& filepath);
    void clear() { m_tokens.clear(); }

    const std::list<Token>& get_tokens() const noexcept { return m_tokens; }

private:
    struct LexerContext
    {
        std::size_t current_line{1};
        std::size_t current_pos{1};

        std::ifstream input_file;

        void open_file(const std::filesystem::path& filepath)
        {
            input_file.open(filepath);
            if (!input_file)
                throw std::runtime_error("[FATAL] Can't open input file!");
        }

        char peek() { return input_file.peek(); }
        char get()
        {
            ++current_pos;
            return input_file.get();
        }
        bool eof() const noexcept { return input_file.eof(); }

    } m_context;

    std::list<Token> m_tokens;

    bool _is_space(const char ch) const noexcept
    {
        return std::isspace(ch) && !_is_newline(ch);
    }
    bool _is_text(const char ch) const noexcept
    {
        return !_is_space(ch) && !_is_newline(ch) && std::isprint(ch);
    }
    bool _is_quot(const char ch) const noexcept { return ch == '\"'; }
    bool _is_newline(const char ch) const noexcept { return ch == '\n'; }
    bool _is_comment(const char ch) const noexcept { return ch == '.'; }
};

void CommandTableLexer::extract_tokens(const std::filesystem::path& filepath)
{
    m_context.open_file(filepath);
    std::string temp;

    while (!m_context.eof())
    {
        while (_is_space(m_context.peek())) // skip spaces
        {
            m_context.get();
        }
        if (_is_comment(m_context.peek())) // skip comments
        {
            while (!_is_newline(m_context.peek()))
                m_context.get();
        }
        if (_is_newline(m_context.peek())) // found new line
        {
            m_context.get();
            ++m_context.current_line;
            m_context.current_pos = 1;
            continue;
        }
        else if (_is_quot(m_context.peek())) // read quoted text
        {
            temp += m_context.get();
            while (!_is_quot(m_context.peek()))
            {
                temp += m_context.get();
            }
            temp += m_context.get();
        }
        else if (_is_text(m_context.peek())) // read word
        {
            while (_is_text(m_context.peek()))
            {
                temp += m_context.get();
                ++m_context.current_pos;
            }
        }

        m_tokens.emplace_back(temp, m_context.current_line,
                              m_context.current_pos);
        temp.clear();
    }

    // remove checksums
    m_tokens.erase(std::find(m_tokens.cbegin(), m_tokens.cend(), "Checksum_A:"),
                   m_tokens.end());
}

class CommandTableParser final
{
    using header_t = std::list<std::pair<std::string, std::string>>;

public:
    CommandTableParser(const std::filesystem::path& input);

    void parse();

    const header_t& get_header() const noexcept { return m_header; }

private:
    std::filesystem::path m_command_table_path;

    CommandTableLexer m_lexer;

    header_t m_header{};
    // cos_interface_t m_cos_interface
    // components_t m_component
    // commands_t m_commands
};

CommandTableParser::CommandTableParser(const std::filesystem::path& input)
    : m_command_table_path(input), m_header() {};

void CommandTableParser::parse()
{
    m_lexer.extract_tokens(m_command_table_path);

    if (m_lexer.get_tokens().empty())
        throw std::runtime_error("[FATAL] Tokens list is empty!");

    auto current_token = m_lexer.get_tokens().cbegin();

    while (*current_token != "COS")
    {
        auto& key = *current_token;
        auto& value = *(++current_token);
        m_header.emplace_back(key.value, value.value);

        ++current_token;
    }
}

int main()
{
    std::filesystem::path command_table_input_filename = "Command_table";
    std::filesystem::path command_table_output_filename =
        "Command_table_output";

    CommandTableParser command_table(command_table_input_filename);
    command_table.parse();

    for (const auto& [key, value] : command_table.get_header())
    {
        std::clog << std::format("{:<45}{}\n", key, value);
    }

    return EXIT_SUCCESS;
}
