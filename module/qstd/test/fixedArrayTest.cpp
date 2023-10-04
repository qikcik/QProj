#include "gtest/gtest.h"
#include "fixedArray.hpp"

TEST(FixedArrayTest, DummyTest2)
{
    qstd::FixedArray<int,20> arr;

    qstd::FixedArray<int,20> a = arr;

    for(auto& el : arr)
    {

    }


    EXPECT_TRUE(true);
}