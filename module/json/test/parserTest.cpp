#include "gtest/gtest.h"
#include "parser.hpp"

using namespace json;

TEST(Parse, SimpleTest)
{
    std::string json = R"({"a":"b","c":{"a":32}})";

    auto res = Parser().parse(json);
    auto out =  std::get_if<Object>(&res);
    EXPECT_EQ(out->get<Value>("a").get<String>(), "b");
    EXPECT_FLOAT_EQ(out->get<Object>("c").get<Value>("a").get<Float>(), 32);
}

TEST(Parse, ArrayTest)
{
    std::string json = R"({"a":[{"b":2.3},{"d":.32}]})";

    auto res = Parser().parse(json);
    auto out =  std::get_if<Object>(&res);
    EXPECT_FLOAT_EQ(out->get<Array>("a").get<Object>(0).get<Value>("b").get<Float>(), 2.3);
    EXPECT_FLOAT_EQ(out->get<Array>("a").get<Object>(1).get<Value>("d").get<Float>(), 0.32);
}