#include "TestRuleOnCode.h"
#include "rules/custom/NothingRule.cpp"

TEST(NothingRuleTest, PropertyTest)
{
    NothingRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("Nothing", rule.name());
    EXPECT_EQ("custom", rule.category());
}

TEST(NothingRuleTest, FirstFailingTest)
{
    EXPECT_FALSE("Start writing a new test");
}
