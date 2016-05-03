#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "CAPA/Version.h"

using namespace ::testing;
using namespace CAPA;

TEST(VersionTest, VersionString)
{
    EXPECT_THAT(Version::identifier(), StrEq("0.10.2"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
