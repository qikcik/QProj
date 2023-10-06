#include "gtest/gtest.h"
#include <vector>
#include <cstddef>
#include <algorithm>

#include "converter.hpp"
#include "dynamicArray.tpp"


struct Foo2
{
    qstd::DynamicArray<float> nums {};

    static const QStructType staticType;
};

//TODO: make generate by Header Tool
const QStructType Foo2::staticType{"Foo",sizeof(Foo2),{
        GEN_QSTRUCT_FIELD_ENTRY(Foo2,nums),

}};

struct Bar2
{
    qstd::DynamicArray<qstd::DynamicArray<Foo2>> arr {};

    static const QStructType staticType;
};

//TODO: make generate by Header Tool
const QStructType Bar2::staticType{"Bar",sizeof(Bar2),{
    GEN_QSTRUCT_FIELD_ENTRY(Bar2,arr)
}};

TEST(ConverterTest2, Test)
{
    Bar2 bar {};
    bar.arr.push_back( qstd::DynamicArray<Foo2>() );
    bar.arr[0].push_back(Foo2());

    for(int i = 0 ; i != 18 ; i++)
    {
        bar.arr[0][0].nums.push_back(i*2);
    }

    auto json = Converter::qstructToJson(bar);
    auto source = json.stringify();
    Bar2 bar2 = Converter::jsonToQStruct<Bar2>(source);


    for(int i = 0 ; i != 18 ; i++)
    {
        EXPECT_EQ(bar.arr[0][0].nums[i], i*2);
    }

}
