#include "gtest/gtest.h"
#include <vector>
#include <cstddef>
#include <algorithm>

#include "qstructType.hpp"
#include "qstructField.hpp"
#include "abandon/fixedArray.hpp"

struct FooArr
{
    qstd::FixedArray<float,32> arr {};

    static const QStructType staticType;
};

//TODO: make generate by Header Tool
const QStructType FooArr::staticType{"Foo",{
        GEN_QSTRUCT_FIELD_ENTRY(FooArr,arr),
}};


TEST(QStructTest, ArrayCheck)
{
    FooArr obj {};
    //obj.arr.push_back(2);
    //obj.arr.push_back(3);
    //obj.arr.push_back(6);
}