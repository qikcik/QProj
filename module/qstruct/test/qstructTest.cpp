#include "gtest/gtest.h"
#include <vector>
#include <cstddef>
#include <algorithm>

#include "qstructType.hpp"
#include "qstructField.hpp"


struct FooBase
{
    float x;
    float y;
    float z;

    static const QStructType staticType;
};

//TODO: make generate by Header Tool
const QStructType FooBase::staticType{"FooBase",sizeof(FooBase),{
        GEN_QSTRUCT_FIELD_ENTRY(FooBase,x),
        GEN_QSTRUCT_FIELD_ENTRY(FooBase,y),
        GEN_QSTRUCT_FIELD_ENTRY(FooBase,z),
}};

struct Foo : public FooBase
{
    std::string text;

    static const QStructType staticType;
};

//TODO: make generate by Header Tool
const QStructType Foo::staticType{"Foo",sizeof(Foo),{
    GEN_QSTRUCT_FIELD_ENTRY(Foo,text)
},&FooBase::staticType};

struct Bar
{
    Foo inner {};

    static const QStructType staticType;
};

//TODO: make generate by Header Tool
const QStructType Bar::staticType{"Bar",sizeof(Bar),{
    GEN_QSTRUCT_FIELD_ENTRY(Bar,inner)
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