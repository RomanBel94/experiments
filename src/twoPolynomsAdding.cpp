#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <stdexcept>
#include <string>

namespace LexerParser
{
enum class TokenType : unsigned char
{
    Undefined,
    Number,
    Newline,
    Eof
};

// ------------------------------------------------------------------------

struct Token
{
    const int value;
    const TokenType type;

    explicit Token(const TokenType type, const int value = 0)
        : type(type), value(value)
    {
    }
    ~Token() = default;

    inline bool operator==(TokenType rhs) const;
    inline bool operator!=(TokenType rhs) const;
    inline operator TokenType() const;
    inline operator int() const;

private:
    Token() = delete;
    Token(const Token&) = delete;
    Token(Token&&) noexcept = delete;
    const Token& operator=(const Token&) = delete;
    Token&& operator=(Token&&) noexcept = delete;
};

inline bool Token::operator==(TokenType rhs) const { return this->type == rhs; }

inline bool Token::operator!=(TokenType rhs) const { return this->type != rhs; }

inline Token::operator TokenType() const { return type; }

inline Token::operator int() const { return value; }

// ------------------------------------------------------------------------

class Lexer
{
private:
    std::ifstream input_file;
    std::string current_token_value;
    std::list<Token> tokens;
    size_t current_line;
    TokenType current_token_type;
    char current_char;

    void skip_spaces();
    void read_token();
    void define_token_type();
    void add_token();
    void add_eof();
    void reset_current_token();
    bool is_number(const std::string& token);

public:
    Lexer();
    ~Lexer() = default;

    inline void parse();
    inline const std::list<Token>& get_tokens() const;
};

Lexer::Lexer()
    : current_token_type(TokenType::Undefined), current_token_value(""),
      current_char('\0'), current_line(1)
{
    input_file.open("polynoms.txt", std::ios::in);
    if (!input_file.is_open())
        throw std::runtime_error("[FATAL] Can't open input file!");

    current_char = input_file.get();
}

void Lexer::skip_spaces()
{
    while (isspace(current_char) && current_char != '\n')
        current_char = input_file.get();
}

void Lexer::read_token()
{
    skip_spaces();
    if (input_file.eof())
    {
        add_eof();
        return;
    }
    if (current_char == '\n')
    {
        current_token_value += current_char;
        ++current_line;
    }
    else
    {
        while (!isspace(current_char) && !input_file.eof())
        {
            current_token_value += current_char;
            current_char = input_file.get();
        }
    }
    add_token();
}

void Lexer::define_token_type()
{
    if (current_token_value == "\n")
        current_token_type = TokenType::Newline;
    else if (is_number(current_token_value))
        current_token_type = TokenType::Number;
    else
        throw std::runtime_error("[FATAL] Invalid token \"" +
                                 current_token_value +
                                 "\" at line: " + std::to_string(current_line));
}

void Lexer::add_token()
{
    define_token_type();

    if (current_token_type == TokenType::Number)
        tokens.emplace_back(current_token_type, std::stoi(current_token_value));
    else if (current_token_type == TokenType::Newline)
        tokens.emplace_back(current_token_type, '\n');

    reset_current_token();
}

void Lexer::add_eof()
{
    current_token_type = TokenType::Eof;
    tokens.emplace_back(current_token_type);
}

void Lexer::reset_current_token()
{
    if (current_token_type == TokenType::Newline)
        current_char = input_file.get();

    current_token_type = TokenType::Undefined;
    current_token_value.clear();
}

bool Lexer::is_number(const std::string& token)
{
    for (char symbol : token)
        if (!isdigit(symbol))
            return false;

    return true;
}

inline void Lexer::parse()
{
    while (current_token_type != TokenType::Eof)
        read_token();

    if (tokens.size() < 7)
        throw std::runtime_error("[FATAL] Invalid input (too few numbers)!");
}

inline const std::list<Token>& Lexer::get_tokens() const { return tokens; }

// ------------------------------------------------------------------------

class Parser
{
private:
    Lexer lexer;
    std::map<int, int> polynom1;
    std::map<int, int> polynom2;
    std::map<int, int>* current_polynom;
    size_t current_line;
    bool line_added;

    inline void reset();
    inline void switch_polynoms() noexcept;
    inline void add_values(const Token& power, const Token& base);

public:
    Parser();
    ~Parser() = default;

    void parse();
    inline std::map<int, int>& get_polynom1();
    inline std::map<int, int>& get_polynom2();
};

Parser::Parser()
    : current_polynom(&polynom1), current_line(1), line_added(false)
{
}

inline void Parser::reset()
{
    polynom1.clear();
    polynom2.clear();
}

inline void Parser::switch_polynoms() noexcept
{
    current_polynom = &polynom2;
    current_line += 2;
}

inline void Parser::add_values(const Token& power, const Token& base)
{
    current_polynom->emplace(power, base);
    line_added = true;
}

void Parser::parse()
{
    lexer.parse();

    auto current_token{lexer.get_tokens().begin()};
    auto next_token{current_token};
    ++next_token;

    for (; (*next_token) != LexerParser::TokenType::Eof;
         ++current_token, ++next_token)
    {
        if (*current_token == TokenType::Number &&
            *next_token == TokenType::Number)
        {
            if (!line_added)
                add_values(*current_token, *next_token);
            else
                throw std::runtime_error(
                    "[FATAL] Unexpected token: \"" +
                    std::to_string((*next_token).value) +
                    "\" at line: " + std::to_string(current_line));
        }
        else if (*current_token == TokenType::Newline &&
                 *next_token == TokenType::Newline)
        {
            switch_polynoms();
            line_added = false;
        }
        else if (*current_token == TokenType::Newline)
        {

            ++current_line;
            line_added = false;
        }
    }
}

inline std::map<int, int>& Parser::get_polynom1() { return polynom1; }

inline std::map<int, int>& Parser::get_polynom2() { return polynom2; }

// -----------------------------------------------------------------------

class PolynomProcessor
{
private:
    Parser parser;
    std::map<int, int>& polynom1;
    std::map<int, int>& polynom2;

    PolynomProcessor(const PolynomProcessor&) = delete;
    PolynomProcessor(PolynomProcessor&&) noexcept = delete;
    PolynomProcessor& operator=(const PolynomProcessor&) = delete;
    PolynomProcessor&& operator=(PolynomProcessor&&) noexcept = delete;

    inline size_t find_max_key();

public:
    PolynomProcessor();
    ~PolynomProcessor() = default;

    template <class Binary_Operator>
    std::map<int, int> operator()(Binary_Operator&& op);
};

PolynomProcessor::PolynomProcessor()
    : polynom1(parser.get_polynom1()), polynom2(parser.get_polynom2()) {};

inline size_t PolynomProcessor::find_max_key()
{
    return std::max((*(polynom1.crbegin())).first,
                    (*(polynom2.crbegin())).first);
}

template <class Binary_Operator>
std::map<int, int> PolynomProcessor::operator()(Binary_Operator&& op)
{
    std::map<int, int> result;
    parser.parse();
    size_t iterations = find_max_key();

    for (size_t i{0}; i <= iterations; ++i)
    {
        result[i] = op(polynom1[i], polynom2[i]);
    }
    return result;
}
}; // namespace LexerParser

int main()
{
    try
    {
        LexerParser::Lexer lexer;
        lexer.parse();
        size_t token_number{1};

        std::cout << "[DEBUG] Lexer has read theese tokens:\n";
        for (const LexerParser::Token& token : lexer.get_tokens())
        {
            switch (token.type)
            {
            case LexerParser::TokenType::Newline:
                std::cout << "[DEBUG] " << token_number
                          << ". Token type: Newline; "
                          << "token value: \\n\n";
                break;
            case LexerParser::TokenType::Number:
                std::cout << "[DEBUG] " << token_number
                          << ". Token type: Number; "
                          << "token value: " << token.value << '\n';
                break;
            case LexerParser::TokenType::Eof:
                std::cout << "[DEBUG] " << token_number << ". Token type: Eof; "
                          << "token value: eof()\n";
                break;
            default:
                std::cout << "[DEBUG] " << token_number
                          << ". Token type: Undefined; "
                          << "token value: " << token.value << '\n';
            }
            ++token_number;
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << '\n';
        exit(EXIT_FAILURE);
    }

    try
    {
        LexerParser::Parser parser;
        parser.parse();

        std::cout << "[DEBUG] Polynom1: \n";
        for (const auto& [power, base] : parser.get_polynom1())
            std::cout << "[DEBUG] Power: " << power << ", base: " << base
                      << '\n';

        std::cout << "[DEBUG] Polynom2: \n";
        for (const auto& [power, base] : parser.get_polynom2())
            std::cout << "[DEBUG] Power: " << power << ", base: " << base
                      << '\n';
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << '\n';
        exit(EXIT_FAILURE);
    }

    try
    {
        LexerParser::PolynomProcessor proc;
        auto result = proc(std::plus());

        std::cout << "[DEBUG] Result: \n";
        for (const auto& [power, base] : result)
            std::cout << "[DEBUG] Power: " << power << ", base: " << base
                      << '\n';
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << '\n';
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}