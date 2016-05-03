#include "TestRuleOnCode.h"
#include "rules/Parallel/ReduceRuleRule.cpp"

TEST(ReduceRuleRuleTest, PropertyTest)
{
    ReduceRuleRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("ReduceRule", rule.name());
    EXPECT_EQ("Parallel", rule.category());
}

TEST(ReduceRuleRuleTest, FirstFailingTest)
{
    EXPECT_FALSE("Start writing a new test");
}
