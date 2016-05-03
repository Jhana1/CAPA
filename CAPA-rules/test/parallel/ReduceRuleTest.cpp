#include "TestRuleOnCode.h"
#include "rules/Parallel/ReduceRule.cpp"

TEST(ReduceRuleTest, PropertyTest)
{
    ReduceRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("Reduce", rule.name());
    EXPECT_EQ("Parallel", rule.category());
}

TEST(ReduceRuleTest, FirstFailingTest)
{
    EXPECT_FALSE("Start writing a new test");
}
