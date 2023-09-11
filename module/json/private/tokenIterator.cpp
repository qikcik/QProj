#include "tokenIterator.hpp"

TokenIterator::TokenIterator(const std::string& inSource)
: source(inSource)
{

}

inline bool isKeyword(const std::string::value_type& c)
{
    return c == '{' or c == '}' or c == '[' or c == ']' or c == ':' or c == ',';
}

const Token::type& TokenIterator::next()
{
    while(position < source.size())
    {
        auto charIt = source[position];

        if(isspace(charIt))
        {
            position++;
            continue;
        }
        else if(isKeyword(charIt))
        {
            position++;
            currentToken = Token::Keyword{charIt};
            return currentToken;
        }
        else if(charIt == '"')
        {
            position++;
            currentToken = tokenizeString();
            return currentToken;
        }
        else if(isdigit(charIt) || charIt == '.')
        {
            currentToken = tokenizeNumber();
            return currentToken;
        }
        else
            position++;
    }
    currentToken =  Token::End{};
    return currentToken;
}

Token::type TokenIterator::tokenizeNumber()
{
    std::string summed;
    bool dot = false;
    while(position != source.size())
    {
        auto charIt = source[position];

        if(isdigit(charIt))
        {
            summed += charIt;
            position++;
        }
        else if(charIt == '.' && !dot)
        {
            dot = true;
            summed += charIt;
            position++;
        }
        else
        {
            break;
        }
    }
    return Token::String(summed);
}

Token::type TokenIterator::tokenizeString()
{
    std::string result;
    while(position != source.size())
    {
        auto charIt = source[position];
        if(charIt == '"')
        {
            position++;
            break;
        }
        result += charIt;
        position++;
    }
    return Token::String(result);
}

const Token::type& TokenIterator::current() const
{
    return currentToken;
}
