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
    GEN_QSTRUCT_FIELD_ENTRY(Foo,x)
    GEN_QSTRUCT_FIELD_ENTRY(Foo,y)
    GEN_QSTRUCT_FIELD_ENTRY(Foo,z)
    GEN_QSTRUCT_FIELD_ENTRY(Foo,text)
}};

TEST(QStructTest, TypeInfoCheck)
{
    EXPECT_EQ(Foo::staticType.getField("x").type,FieldType::Float);
    EXPECT_EQ(Foo::staticType.getField("y").type,FieldType::Float);
    EXPECT_EQ(Foo::staticType.getField("z").type,FieldType::Float);

    EXPECT_EQ(Foo::staticType.getField("text").type,FieldType::StdString);
}

TEST(QStructTest, GetFieldAndSetField)
{
    Foo obj = {2,3,4,"test"};

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