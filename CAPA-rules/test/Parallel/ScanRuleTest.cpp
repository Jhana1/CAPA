#include "TestRuleOnCode.h"
#include "rules/Parallel/ScanRule.cpp"

TEST(ScanRuleTest, PropertyTest)
{
    ScanRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("Scan", rule.name());
    EXPECT_EQ("Parallel", rule.category());
}

TEST(ScanRuleTest, FirstFailingTest)
{
    EXPECT_FALSE("Start writing a new test");
}
