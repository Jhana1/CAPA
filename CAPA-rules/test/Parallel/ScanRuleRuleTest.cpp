#include "TestRuleOnCode.h"
#include "rules/Parallel/ScanRuleRule.cpp"

TEST(ScanRuleRuleTest, PropertyTest)
{
    ScanRuleRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("ScanRule", rule.name());
    EXPECT_EQ("Parallel", rule.category());
}

TEST(ScanRuleRuleTest, FirstFailingTest)
{
    EXPECT_FALSE("Start writing a new test");
}
