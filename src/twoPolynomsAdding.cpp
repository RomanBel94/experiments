#include <fstream>
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

        Token() = delete;
        explicit Token(const TokenType type, const std::string& value = "") :
            type(type), value(value) {}
        ~Token() = default;

    private:
        Token(const Token&) = delete;
        Token(Token&&) noexcept = delete;
        const Token& Token::operator=(const Token&) = delete;
        Token&& Token::operator=(Token&&) noexcept = delete;
    };

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

        void parse();
        inline const std::list<Token>& get_tokens() const;
    };

    Lexer::Lexer() : current_token_type(TokenType::Undefined), current_token_value(""), current_char('\0')
    {
        input_file.open("polynoms.txt", std::ios::in);
        if (!input_file.is_open())
            std::cout << "Can't open input file!" << std::endl;

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
            current_token_value += current_char;
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

    void Lexer::parse()
    {
        if (!input_file.is_open())
            return;

        while (current_token_type != TokenType::Eof)
            read_token();
    }

    inline const std::list<Token>& Lexer::get_tokens() const
    {
        return tokens;
    }

    // ------------------------------------------------------------------------

    class Parser
    {
    private:
        Lexer lexer;
    };
};

int main()
{
    LexerParser::Lexer lexer;
    lexer.parse();
    size_t token_number{ 0 };

    for (const LexerParser::Token& token : lexer.get_tokens())
    {
        switch (token.type)
        {
        case LexerParser::TokenType::Undefined:
            std::cout << token_number << ". Token type: Undefined; "
                << "token value: " << token.value << '\n';
            break;
        case LexerParser::TokenType::Forbiden:
            std::cout << token_number << ". Token type: Forbiden; "
                << "token value: " << token.value << '\n';
            break;
        case LexerParser::TokenType::Newline:
            std::cout << token_number << ". Token type: Newline; "
                << "token value: \\n\n";
            break;
        case LexerParser::TokenType::Number:
            std::cout << token_number << ". Token type: Number; "
                << "token value: " << token.value << '\n';
            break;
        case LexerParser::TokenType::Eof:
            std::cout << token_number << ". Token type: Eof; "
                << "token value: eof()";
            break;
        }
        ++token_number;
    }

    return 0;
}