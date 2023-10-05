#include "gtest/gtest.h"
#include "dynamicArray.hpp"
#include "dynamicArray.tpp"

TEST(DynamicArrayTest, Test0)
{
    qstd::DynamicArray<int> arr;
    arr.reserve(2);
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    arr.push_back(4);

    const qstd::DynamicArray<int>& arr2 = arr;

    for(const auto it : arr2)
    {
        std::cout << it << std::endl;
    }


    EXPECT_TRUE(true);
}