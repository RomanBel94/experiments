#include <algorithm>
#include <array>
#include <deque>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

using namespace std::literals;

enum class TokenType : unsigned char
{
    TOKEN_UNDEF,
    TOKEN_STRING,
    TOKEN_QUOTED_STRING,
    TOKEN_SOLO_DIVIDER,
    TOKEN_DOUBLE_DIVIDER,
    TOKEN_NEWLINE,
    TOKEN_OPERATOR,
    TOKEN_MANOUVERE,
    TOKEN_NUMBER,
    TOKEN_KEYWORD,
};

class Token
{
public:
    Token(const std::string_view value, std::size_t line = 1,
          std::size_t pos = 1, TokenType type = TokenType::TOKEN_UNDEF)
        : m_value{value}, m_line{line}, m_pos{pos - value.size()}, m_type{type}
    {
    }

    Token(char ch, std::size_t line = 1, std::size_t pos = 1,
          TokenType type = TokenType::TOKEN_UNDEF)
        : m_value{1, ch}, m_line{line}, m_pos{pos - 1}, m_type{type}
    {
    }

    std::string to_string() const noexcept
    {
        return std::format(
            "Value: {:<35}line: {:<10}pos: {:<10} type: {}",
            ((m_value == "\x1\n" || m_value == "\n") ? "\\n" : m_value), m_line,
            m_pos, get_type_string());
    }

    TokenType type() const noexcept { return m_type; }
    std::string value() const noexcept { return m_value; }

    auto operator<=>(const Token&) const = default;

    bool operator==(const std::string_view rhs) const noexcept
    {
        return m_value == rhs;
    }

    std::string_view get_type_string() const
    {
        std::string_view ret;

        switch (m_type)
        {
        case TokenType::TOKEN_UNDEF:
            ret = "TOKEN_UNDEF";
            break;
        case TokenType::TOKEN_STRING:
            ret = "TOKEN_STRING";
            break;
        case TokenType::TOKEN_QUOTED_STRING:
            ret = "TOKEN_QUOTED_STRING";
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
        case TokenType::TOKEN_KEYWORD:
            ret = "TOKEN_KEYWORD";
            break;
        }
        return ret;
    }

private:
    std::string m_value;
    std::size_t m_line;
    std::size_t m_pos;
    TokenType m_type;
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

        std::array<std::string_view, 12> keywords{
            "COS"sv,       "interface"sv,  "Main"sv,    "objects"sv,
            "Component"sv, "Components"sv, "Command"sv, "Commands"sv,
            "End"sv,       "of"sv,         "command"sv, "table"sv};

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
                         TokenType::TOKEN_QUOTED_STRING);
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
            else if (std::ranges::find(keywords, temp) != keywords.cend())
                type = TokenType::TOKEN_KEYWORD;

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
            m_tokens.emplace_back(m_lexer_context.process_newline());
        }
        else if (m_lexer_context.peek() == '\"') // read quoted text
        {
            m_tokens.emplace_back(m_lexer_context.process_quoted_text());
        }
        else if (_is_text(m_lexer_context.peek())) // read word
        {
            m_tokens.emplace_back(m_lexer_context.process_text());
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
public:
    CommandTableParser(const fs::path& input);

    void parse();

private:
    fs::path m_command_table_path;

    CommandTableLexer m_lexer;

    using header_unit_t = std::pair<const std::string, const std::string>;
    using header_t = std::deque<header_unit_t>;
    using token_iterator_t = decltype(m_lexer.get_tokens().cbegin());

    header_t m_header{};
    // cos_interface_t m_cos_interface
    // components_t m_component
    // commands_t m_commands

    struct HeaderParsingContext
    {
        std::string key;
        std::string value;

        void add_key(const std::string& str) { key = str; }
        void add_value(const std::string& str) { value = str; }

        [[nodiscard]] header_unit_t commit_unit()
        {
            header_unit_t unit{key, value};
            key.clear();
            value.clear();
            return unit;
        }
    } m_header_parsing_context;

    void _throw_unexpected_token(token_iterator_t& it)
    {
        throw std::runtime_error(
            std::format("[FATAL]:\tUnexpected token:\n{}\n", it->to_string()));
    }

    void _parse_header(token_iterator_t& it)
    {
        while (it->type() != TokenType::TOKEN_KEYWORD)
        {
            _process_header_key(it);
            ++it;
        }
    }

    void _process_header_key(token_iterator_t& it)
    {
        // skip empty lines
        while (it->type() == TokenType::TOKEN_NEWLINE)
            ++it;

        switch (it->type())
        {
        case TokenType::TOKEN_STRING:
        {
            m_header_parsing_context.add_key(it->value());
            ++it;
            _process_header_value(it);
            return;
        }
        case TokenType::TOKEN_KEYWORD:
            return;
        default:
            _throw_unexpected_token(it);
        }
    }

    void _process_header_value(token_iterator_t& it)
    {
        switch (it->type())
        {
        case TokenType::TOKEN_QUOTED_STRING:
            m_header_parsing_context.add_value(it->value());
            ++it;
            _finish_header_unit(it);
            return;
        default:
            _throw_unexpected_token(it);
        }
    }

    void _finish_header_unit(token_iterator_t& it)
    {
        switch (it->type())
        {
        case TokenType::TOKEN_NEWLINE:
            m_header.emplace_back(m_header_parsing_context.commit_unit());
            return;
        default:
            _throw_unexpected_token(it);
        }
    }

public:
    const header_t& get_header() const noexcept { return m_header; }
};

CommandTableParser::CommandTableParser(const fs::path& input)
    : m_command_table_path(input), m_header() {};

void CommandTableParser::parse()
{

    m_lexer.extract_tokens(m_command_table_path);

    if (m_lexer.get_tokens().empty())
        throw std::runtime_error("[FATAL] Tokens list is empty!");

    auto current_token = m_lexer.get_tokens().cbegin();

    _parse_header(current_token);

    std::cout << "Collected header tokens:\n";
    std::ranges::for_each(
        m_header, [](const auto& pair)
        { std::cout << std::format("{:<40}{}\n", pair.first, pair.second); });
}

int main()
{
    fs::path command_table_input = "Command_table";
    fs::path command_table_output = "Command_table_output";

    // DEBUG
    // CommandTableLexer lexer;
    // std::clog << std::format("Start parsing {}\n",
    //                          command_table_input.string());
    // lexer.extract_tokens(command_table_input);

    // std::clog << std::format("Start writing {}\n",
    //                         command_table_output.string());
    // std::ofstream output_file{command_table_output};
    // for (const auto& token : lexer.get_tokens())
    //    output_file << token.to_string() << '\n';

    try
    {
        CommandTableParser parser(command_table_input);
        parser.parse();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
