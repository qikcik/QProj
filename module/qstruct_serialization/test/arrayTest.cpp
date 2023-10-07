#include "gtest/gtest.h"
#include <vector>
#include <cstddef>
#include <algorithm>

#include "qstructType.hpp"
#include "qstructField.hpp"
#include "dynamicArray.tpp"
#include "converter.hpp"

struct FooArr
{
    qstd::DynamicArray<float> arr {};

    static const QStructType staticType;
};

//TODO: make generate by Header Tool
const QStructType FooArr::staticType{"Foo",sizeof(FooArr),{
        GEN_QSTRUCT_FIELD_ENTRY(FooArr,arr),
}};




TEST(ConverterTest, ArrayCheck)
{
    FooArr obj {};
    //obj.arr.push_back(2);
    //obj.arr.push_back(3);
    //obj.arr.push_back(6);

    auto json = Converter::qstructToJson(obj);
    auto source = json.stringify();
    FooArr obj2 = Converter::jsonToQStruct<FooArr>(source);
}