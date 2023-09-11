#include "gtest/gtest.h"
#include "object.hpp"

using namespace json;

TEST(EntryTest, EntryTest)
{
    Object e;
    e = Object{ {
        {"a",Value{"b"}},
        {"b",Value{"32"}},
        {"c",Object{{
            {"a",Value{"b"}},
            {"d",Value{"32"}},
            {"v",Array{{
                Object{{{"i", Value{"0"}}}},
                Object{{{"i", Value{"1"}}}},
                }}},
            {"v2",Array{{
                Value{"test"}, Value{"test1"}, Value{"test3"}
                }}}
            }}
        },
    } };

    EXPECT_EQ(e.get<Value>("a").value, "b");
    EXPECT_EQ(e.get<Value>("b").value, "32");
    EXPECT_EQ(e.get<Object>("c").get<Value>("a").value, "b");
    EXPECT_EQ(e.get<Object>("c").get<Value>("d").value, "32");
    EXPECT_EQ(e.get<Object>("c").get<Array>("v").get<Object>(0).get<Value>("i").value, "0");
    EXPECT_EQ(e.get<Object>("c").get<Array>("v").get<Object>(1).get<Value>("i").value, "1");
    EXPECT_EQ(e.get<Object>("c").get<Array>("v2").get<Value>(1).value, "test1");

    //std::cout << e.stringify() << std::endl;
}