#pragma once
#include <string>
#include <variant>


namespace Token
{
    struct End
    {

    };

    struct Keyword
    {
        std::string::value_type value;
    };

    struct String
    {
        std::string value;
    };

    struct Float
    {
        float value;
    };

    using type = std::variant<End,Keyword,String,Float>;
}


class TokenIterator
{
public:
    explicit TokenIterator(const std::string& inSource);

    const Token::type& next();
    const Token::type& current() const;

    int getPosition() const {return position;};

protected:
    int position {0};
    const std::string& source;
    Token::type currentToken {Token::End{}};

    Token::type tokenizeString();
    Token::type tokenizeNumber();
};