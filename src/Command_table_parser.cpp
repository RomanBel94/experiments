#include <algorithm>
#include <deque>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

enum class TokenType : unsigned char
{
    TOKEN_UNDEF,
    TOKEN_STRING,
    TOKEN_SOLO_DIVIDER,
    TOKEN_DOUBLE_DIVIDER,
    TOKEN_NEWLINE,
    TOKEN_OPERATOR,
    TOKEN_MANOUVERE,
    TOKEN_NUMBER,
    NUM_TOKENS
};

class Token
{
public:
    Token(const std::string_view value, std::size_t line = 1,
          std::size_t pos = 1, TokenType type = TokenType::TOKEN_UNDEF)
        : value{value}, line{line}, pos{pos - value.size()}, type{type}
    {
    }

    Token(char ch, std::size_t line = 1, std::size_t pos = 1,
          TokenType type = TokenType::TOKEN_UNDEF)
        : value{1, ch}, line{line}, pos{pos - 1}, type{type}
    {
    }

    std::string to_string() const noexcept
    {
        return std::format(
            "Value: {:<35}line: {:<10}pos: {:<10} type: {}",
            ((value == "\x1\n" || value == "\n") ? "\\n" : value), line, pos,
            _get_type_string());
    }

    auto operator<=>(const Token&) const = default;

    bool operator==(const std::string_view rhs) const noexcept
    {
        return value == rhs;
    }

private:
    std::string value;
    std::size_t line;
    std::size_t pos;
    TokenType type;

    std::string_view _get_type_string() const
    {
        std::string_view ret;

        switch (type)
        {
        case TokenType::TOKEN_UNDEF:
            ret = "TOKEN_UNDEF";
            break;
        case TokenType::TOKEN_STRING:
            ret = "TOKEN_STRING";
            break;
        case TokenType::TOKEN_SOLO_DIVIDER:
            ret = "TOKEN_SOLO_DIVIDER";
            break;
        case TokenType::TOKEN_DOUBLE_DIVIDER:
            ret = "TOKEN_DOUBLE_DIVIDER";
            break;
        case TokenType::TOKEN_NEWLINE:
            ret = "TOKEN_NEWLINE";
            break;
        case TokenType::TOKEN_OPERATOR:
            ret = "TOKEN_OPERATOR";
            break;
        case TokenType::TOKEN_MANOUVERE:
            ret = "TOKEN_MANOUVERE";
            break;
        case TokenType::TOKEN_NUMBER:
            ret = "TOKEN_NUMBER";
            break;
        }
        return ret;
    }
};

class CommandTableLexer
{
public:
    void extract_tokens(const fs::path& filepath);
    void clear() { m_tokens.clear(); }

    const std::deque<Token>& get_tokens() const noexcept { return m_tokens; }

private:
    struct LexerContext
    {
        std::size_t current_line{1};
        std::size_t current_pos{1};

        std::ifstream input_file;

        void open_file(const fs::path& filepath)
        {
            input_file.open(filepath);
            if (!input_file)
                throw std::runtime_error("[FATAL] Can't open input file!");
        }
        void close_file() noexcept { input_file.close(); }

        char peek() noexcept { return input_file.peek(); }
        char get() noexcept
        {
            ++current_pos;
            return input_file.get();
        }
        bool eof() const noexcept { return input_file.eof(); }

        void skip_comment() // skip comments
        {
            while (peek() != '\n')
                get();
            get();
        }

        Token process_newline() noexcept
        {
            Token result(get(), current_line, current_pos,
                         TokenType::TOKEN_NEWLINE);
            ++current_line;
            current_pos = 1;
            return result;
        }

        Token process_quoted_text() noexcept
        {
            std::string temp;

            temp += get();
            while (peek() != '\"')
                temp += get();
            temp += get();

            return Token(temp, current_line, current_pos,
                         TokenType::TOKEN_STRING);
        }

        Token process_text() noexcept
        {
            std::string temp;
            TokenType type{TokenType::TOKEN_STRING};

            while (!std::isspace(peek()))
            {
                temp += get();
                ++current_pos;
            }
            if (temp == "|")
                type = TokenType::TOKEN_SOLO_DIVIDER;
            else if (temp == "||")
                type = TokenType::TOKEN_DOUBLE_DIVIDER;
            else if (temp == "=")
                type = TokenType::TOKEN_OPERATOR;
            else if (temp.starts_with("M_"))
                type = TokenType::TOKEN_MANOUVERE;
            else if (std::ranges::all_of(temp, ::isdigit))
                type = TokenType::TOKEN_NUMBER;

            return Token(temp, current_line, current_pos, type);
        }

    } m_lexer_context;

    std::deque<Token> m_tokens;

    bool _is_space(const char ch) const noexcept
    {
        return std::isspace(ch) && ch != '\n';
    }
    bool _is_text(const char ch) const noexcept
    {
        return !_is_space(ch) && std::isprint(ch);
    }
    void _skip_spaces()
    {
        while (_is_space(m_lexer_context.peek())) // skip spaces
            m_lexer_context.get();
    }
    void _make_backup(const fs::path& filepath) const;
};

void CommandTableLexer::_make_backup(const fs::path& filepath) const
{
    auto filepath_copy = std::format("{}.bak", filepath.string());
    fs::remove(filepath_copy);
    fs::copy(filepath, filepath_copy);
}

void CommandTableLexer::extract_tokens(const fs::path& filepath)
{
    _make_backup(filepath);

    m_lexer_context.open_file(filepath);

    while (!m_lexer_context.eof())
    {
        _skip_spaces();
        if (m_lexer_context.peek() == '.') // skip comments
        {
            m_lexer_context.skip_comment();
        }
        else if (m_lexer_context.peek() == '\n') // found new line
        {
            m_tokens.push_back(m_lexer_context.process_newline());
        }
        else if (m_lexer_context.peek() == '\"') // read quoted text
        {
            m_tokens.push_back(m_lexer_context.process_quoted_text());
        }
        else if (_is_text(m_lexer_context.peek())) // read word
        {
            m_tokens.push_back(m_lexer_context.process_text());
        }
    }

    m_lexer_context.close_file();

    // remove checksums
    const auto& checksum =
        std::find(m_tokens.cbegin(), m_tokens.cend(), "Checksum_A:");
    m_tokens.erase(checksum, m_tokens.end());
}

class CommandTableParser final
{
    using header_t =
        std::deque<std::pair<const std::string, const std::string>>;

public:
    CommandTableParser(const fs::path& input);

    void parse();

    const header_t& get_header() const noexcept { return m_header; }

private:
    fs::path m_command_table_path;

    CommandTableLexer m_lexer;

    header_t m_header{};
    // cos_interface_t m_cos_interface
    // components_t m_component
    // commands_t m_commands
};

CommandTableParser::CommandTableParser(const fs::path& input)
    : m_command_table_path(input), m_header() {};

void CommandTableParser::parse()
{

    m_lexer.extract_tokens(m_command_table_path);

    if (m_lexer.get_tokens().empty())
        throw std::runtime_error("[FATAL] Tokens list is empty!");

    auto current_token = m_lexer.get_tokens().cbegin();
}

int main()
{
    fs::path command_table_input = "Command_table";
    fs::path command_table_output = "Command_table_output";

    // DEBUG
    CommandTableLexer lexer;
    std::clog << std::format("Start parsing {}\n",
                             command_table_input.string());
    lexer.extract_tokens(command_table_input);

    // DEBUG
    std::clog << std::format("Start writing {}\n",
                             command_table_output.string());
    std::ofstream output_file{command_table_output};
    for (const auto& token : lexer.get_tokens())
        output_file << token.to_string() << '\n';

    return EXIT_SUCCESS;
}
