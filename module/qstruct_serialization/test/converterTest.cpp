#include "gtest/gtest.h"
#include <vector>
#include <cstddef>
#include <algorithm>

#include "converter.hpp"
#include "dynamicArray.tpp"


struct Foo
{
    float x {};
    float y {};
    float z {};

    std::string text {};

    static const QStructType staticType;
};

//TODO: make generate by Header Tool
const QStructType Foo::staticType{"Foo",sizeof(Foo),{
        GEN_QSTRUCT_FIELD_ENTRY(Foo,x),
        GEN_QSTRUCT_FIELD_ENTRY(Foo,y),
        GEN_QSTRUCT_FIELD_ENTRY(Foo,z),
        GEN_QSTRUCT_FIELD_ENTRY(Foo,text)
}};

struct Bar
{
    std::string text {};
    Foo foo {};
    qstd::DynamicArray<Foo> arr {};

    static const QStructType staticType;
};

//TODO: make generate by Header Tool
const QStructType Bar::staticType{"Bar",sizeof(Bar),{
        GEN_QSTRUCT_FIELD_ENTRY(Bar,text),
        GEN_QSTRUCT_FIELD_ENTRY(Bar,foo),
        GEN_QSTRUCT_FIELD_ENTRY(Bar,arr)
}};

TEST(ConverterTest, Test)
{
    Bar bar {"lorem",{1,2,3,"lorem ipsum"}};
    bar.arr.push_back({1,2,3,"lorem ipsum"});
    bar.arr.push_back({4,5,6,"ipsum lorem"});

    auto json = Converter::qstructToJson(bar);
    auto source = json.stringify();
    Bar bar2 = Converter::jsonToQStruct<Bar>(source);
    EXPECT_EQ(bar2.foo.x,1);
    EXPECT_EQ(bar2.foo.y,2);
    EXPECT_EQ(bar2.foo.z,3);
    EXPECT_EQ(bar2.foo.text,"lorem ipsum");

    EXPECT_EQ(bar2.arr[0].text,"lorem ipsum");
    EXPECT_EQ(bar2.arr[1].text,"ipsum lorem");
}
