#include "TestRuleOnCode.h"
#include "rules/Parallel/MapRuleRule.cpp"

TEST(MapRuleRuleTest, PropertyTest)
{
    MapRuleRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("MapRule", rule.name());
    EXPECT_EQ("Parallel", rule.category());
}

TEST(MapRuleRuleTest, FirstFailingTest)
{
    EXPECT_FALSE("Start writing a new test");
}
