#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <string>

namespace LexerParser
{
enum class TokenType : unsigned char
{
    Undefined,
    Forbiden,
    Number,
    Newline,
    Eof
};

// ------------------------------------------------------------------------

struct Token
{
    const TokenType type;
    const std::string value;

    explicit Token(const TokenType type, const std::string& value = "")
        : type(type), value(value)
    {
    }
    ~Token() = default;

    inline bool operator==(TokenType rhs) const;
    inline bool operator!=(TokenType rhs) const;
    inline operator TokenType() const;

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

// ------------------------------------------------------------------------

class Lexer
{
private:
    TokenType current_token_type;
    std::string current_token_value;
    std::list<Token> tokens;
    std::ifstream input_file;
    char current_char;

    void skip_spaces();
    void read_token();
    void define_token_type(const std::string& token);
    void add_token();
    void add_eof();
    void reset_current_token();
    bool is_number(const std::string& token);

public:
    Lexer();
    ~Lexer();

    inline void parse();
    inline const std::list<Token>& get_tokens() const;
};

Lexer::Lexer()
    : current_token_type(TokenType::Undefined), current_token_value(""),
      current_char('\0')
{
    input_file.open("polynoms.txt", std::ios::in);
    if (!input_file.is_open())
        std::cout << "[FATAL] Can't open input file!" << std::endl;

    current_char = input_file.get();
}

Lexer::~Lexer()
{
    if (input_file.is_open())
        input_file.close();
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

void Lexer::define_token_type(const std::string& token)
{
    if (token == "\n")
        current_token_type = TokenType::Newline;
    else if (is_number(token))
        current_token_type = TokenType::Number;
    else
        current_token_type = TokenType::Forbiden;
}

void Lexer::add_token()
{
    define_token_type(current_token_value);
    tokens.emplace_back(current_token_type, current_token_value);
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
    if (!input_file.is_open())
        return;

    while (current_token_type != TokenType::Eof)
        read_token();
}

inline const std::list<Token>& Lexer::get_tokens() const { return tokens; }

// ------------------------------------------------------------------------

class Parser
{
private:
    Lexer lexer;
    std::map<int, int> polynom1;
    std::map<int, int> polynom2;
    std::map<int, int> result_polynom;
    std::map<int, int>* current_polynom;
    size_t current_line;

    inline void reset();
    inline void add_values(const Token& power, const Token& base);

public:
    Parser();
    ~Parser() = default;

    void parse();
    inline std::map<int, int>& get_polynom1();
    inline std::map<int, int>& get_polynom2();
};

Parser::Parser() : current_polynom(&polynom1), current_line(1) {}

inline void Parser::reset()
{
    polynom1.clear();
    polynom2.clear();
}

inline void Parser::add_values(const Token& power, const Token& base)
{
    current_polynom->emplace(std::stoi(power.value), std::stoi(base.value));
}

void Parser::parse()
{
    lexer.parse();
    if (lexer.get_tokens().size() < 7)
        std::cout << "Invalid input (too few numbers)!\n";

    auto current_token{lexer.get_tokens().begin()};
    auto next_token{current_token};
    ++next_token;

    for (; (*next_token) != LexerParser::TokenType::Eof;
         ++current_token, ++next_token)
    {
        if (*current_token == TokenType::Forbiden)
        {
            std::cout << "[FATAL] Invalid token at line " << current_line
                      << ". Type: Forbiden, "
                      << "value: " << (*current_token).value << '\n';
            reset();
            break;
        }
        else if (*current_token == TokenType::Number &&
                 *next_token == TokenType::Number)
        {
            add_values(*current_token, *next_token);
            ++current_line;
        }
        else if (*current_token == TokenType::Newline &&
                 *next_token == TokenType::Newline)
        {
            if (current_polynom != &polynom2)
            {
                current_polynom = &polynom2;
                current_line += 2;
            }
            else
            {
                std::cout
                    << "[FATAL] Syntax error at line " << current_line
                    << " two polynoms must be divided by one empty line!\n";
                reset();
                break;
            }
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

    template <class Binary_Op>
    std::map<int, int> operator()(Binary_Op&& op);
};

PolynomProcessor::PolynomProcessor()
    : polynom1(parser.get_polynom1()), polynom2(parser.get_polynom2()) {};

inline size_t PolynomProcessor::find_max_key()
{
    return std::max((*(polynom1.crbegin())).first,
                    (*(polynom2.crbegin())).first);
}

template <class Binary_Op>
std::map<int, int> PolynomProcessor::operator()(Binary_Op&& op)
{
    std::map<int, int> result;
    parser.parse();
    size_t iterations = find_max_key();

    for (size_t i{0}; i < iterations; ++i)
    {
        result[i] = op(polynom1[i], polynom2[i]);
    }
    return result;
}
}; // namespace LexerParser

int main()
{
    LexerParser::Lexer lexer;
    lexer.parse();
    size_t token_number{1};

    std::cout << "[DEBUG] Lexer has read theese tokens:\n";
    for (const LexerParser::Token& token : lexer.get_tokens())
    {
        switch (token.type)
        {
        case LexerParser::TokenType::Forbiden:
            std::cout << "[DEBUG] " << token_number
                      << ". Token type: Forbiden; "
                      << "token value: " << token.value << '\n';
            break;
        case LexerParser::TokenType::Newline:
            std::cout << "[DEBUG] " << token_number << ". Token type: Newline; "
                      << "token value: \\n\n";
            break;
        case LexerParser::TokenType::Number:
            std::cout << "[DEBUG] " << token_number << ". Token type: Number; "
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

    LexerParser::Parser parser;
    parser.parse();

    std::cout << "[DEBUG] Polynom1: \n";
    for (const auto& [power, base] : parser.get_polynom1())
        std::cout << "[DEBUG] Power: " << power << ", base: " << base << '\n';

    std::cout << "[DEBUG] Polynom2: \n";
    for (const auto& [power, base] : parser.get_polynom2())
        std::cout << "[DEBUG] Power: " << power << ", base: " << base << '\n';

    LexerParser::PolynomProcessor proc;
    auto result = proc(std::plus());

    std::cout << "[DEBUG] Result: \n";
    for (const auto& [power, base] : result)
        std::cout << "[DEBUG] Power: " << power << ", base: " << base << '\n';

    return 0;
}