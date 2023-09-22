#include "gtest/gtest.h"
#include <vector>
#include <cstddef>
#include <algorithm>

#include "converter.hpp"


struct Foo
{
    float x {};
    float y {};
    float z {};

    std::string text {};

    static const QStructType staticType;
};

//TODO: make generate by Header Tool
const QStructType Foo::staticType{"Foo",{
        GEN_QSTRUCT_FIELD_ENTRY(Foo,x)
        GEN_QSTRUCT_FIELD_ENTRY(Foo,y)
        GEN_QSTRUCT_FIELD_ENTRY(Foo,z)
        GEN_QSTRUCT_FIELD_ENTRY(Foo,text)
}};

TEST(ConverterTest, Test)
{
    Foo foo {1,2,3,"lorem ipsum"};
    auto json = Converter().qstructToJson(foo);
    auto source =  json.stringify();
    Foo bar = Converter().jsonToQStruct<Foo>(source);
    EXPECT_EQ(bar.x,1);
    EXPECT_EQ(bar.y,2);
    EXPECT_EQ(bar.z,3);
    EXPECT_EQ(bar.text,"lorem ipsum");
}
