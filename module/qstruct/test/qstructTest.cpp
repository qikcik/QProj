#include "gtest/gtest.h"
#include <vector>
#include <cstddef>
#include <algorithm>

#include "qstructType.hpp"
#include "qstructField.hpp"

struct Foo
{
    float x;
    float y;
    float z;

    std::string text;

    static const QStructType staticType;
};

//TODO: make generate by Header Tool
const QStructType Foo::staticType{"Foo",{
    GEN_QSTRUCT_FIELD_ENTRY(Foo,x),
    GEN_QSTRUCT_FIELD_ENTRY(Foo,y),
    GEN_QSTRUCT_FIELD_ENTRY(Foo,z),
    GEN_QSTRUCT_FIELD_ENTRY(Foo,text)
}};

struct Bar
{
    Foo inner {};
    std::vector<Foo> arr;


    static const QStructType staticType;
};

//TODO: make generate by Header Tool
const QStructType Bar::staticType{"Bar",{
    GEN_QSTRUCT_FIELD_ENTRY(Bar,inner)
    //(Bar,arr)
}};

TEST(QStructTest, TypeInfoCheck)
{
    EXPECT_TRUE(std::holds_alternative<FieldType::Float>(Foo::staticType.getField("x").type));
    EXPECT_TRUE(std::holds_alternative<FieldType::Float>(Foo::staticType.getField("y").type));
    EXPECT_TRUE(std::holds_alternative<FieldType::Float>(Foo::staticType.getField("z").type));
    EXPECT_TRUE(std::holds_alternative<FieldType::StdString>(Foo::staticType.getField("text").type));
}

TEST(QStructTest, GetFieldAndSetField)
{
    Foo obj = {2,3,4,"test"};

    EXPECT_EQ(Foo::staticType.getField("x").getValueRef<float>(&obj),2);

    EXPECT_EQ(Foo::staticType.getField("x").getValueRef<float>(&obj),2);
    EXPECT_EQ(Foo::staticType.getField("y").getValueRef<float>(&obj),3);
    EXPECT_EQ(Foo::staticType.getField("z").getValueRef<float>(&obj),4);
    EXPECT_EQ(Foo::staticType.getField("text").getValueRef<std::string>(&obj),"test");

    Foo::staticType.getField("text").getValueRef<std::string>(&obj) = "lorem ipsum";
    Foo::staticType.getField("z").getValueRef<float>(&obj) = 10;

    EXPECT_EQ(Foo::staticType.getField("x").getValueRef<float>(&obj),2);
    EXPECT_EQ(Foo::staticType.getField("y").getValueRef<float>(&obj),3);
    EXPECT_EQ(Foo::staticType.getField("z").getValueRef<float>(&obj),10);
    EXPECT_EQ(Foo::staticType.getField("text").getValueRef<std::string>(&obj),"lorem ipsum");
}