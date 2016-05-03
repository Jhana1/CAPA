#include "TestRuleOnCode.h"
#include "rules/Parallel/MapRule.cpp"

TEST(MapRuleTest, PropertyTest)
{
    MapRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("Map", rule.name());
    EXPECT_EQ("Parallel", rule.category());
}

TEST(MapRuleTest, FirstFailingTest)
{
    EXPECT_FALSE("Start writing a new test");
}
