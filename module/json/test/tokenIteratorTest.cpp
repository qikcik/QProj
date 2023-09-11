#include "gtest/gtest.h"
#include "tokenIterator.hpp"


TEST(TokenIterator, SimpleTest)
{
    std::string json = R"({"c":{"v2":["test","test1","test3"]}})";
    TokenIterator it{json};

    EXPECT_EQ(std::get<Token::Keyword>(it.next()).value, '{');
    EXPECT_EQ(std::get<Token::String>(it.next()).value, "c");
    EXPECT_EQ(std::get<Token::Keyword>(it.next()).value, ':');
    EXPECT_EQ(std::get<Token::Keyword>(it.next()).value, '{');
    EXPECT_EQ(std::get<Token::String>(it.next()).value, "v2");
    EXPECT_EQ(std::get<Token::Keyword>(it.next()).value, ':');
    EXPECT_EQ(std::get<Token::Keyword>(it.next()).value, '[');
    EXPECT_EQ(std::get<Token::String>(it.next()).value, "test");
    EXPECT_EQ(std::get<Token::Keyword>(it.next()).value, ',');
    EXPECT_EQ(std::get<Token::String>(it.next()).value, "test1");
    EXPECT_EQ(std::get<Token::Keyword>(it.next()).value, ',');
    EXPECT_EQ(std::get<Token::String>(it.next()).value, "test3");
    EXPECT_EQ(std::get<Token::Keyword>(it.next()).value, ']');
    EXPECT_EQ(std::get<Token::Keyword>(it.next()).value, '}');
    EXPECT_EQ(std::get<Token::Keyword>(it.next()).value, '}');
    std::get<Token::End>(it.next());
}