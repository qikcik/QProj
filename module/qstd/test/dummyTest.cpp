#include "gtest/gtest.h"
#include "fixedArray.hpp"

TEST(DummyTest, DummyTest)
{
    qstd::FixedArray<int,20> arr;

    qstd::FixedArray<int,20> a = arr;

    for(auto& el : arr)
    {

    }


    EXPECT_TRUE(true);
}