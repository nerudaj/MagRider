#include "catch_amalgamated.hpp"
#include "misc/Utility.hpp"

TEST_CASE("[Utility]")
{
    SECTION("setBestTime: Returns true")
    {
        SaveState save;

        SECTION("When no time was recorded prior")
        {
            REQUIRE(Utility::setBestTime(save, 0, 1.f));
        }

        SECTION("When new best is set")
        {
            Utility::setBestTime(save, 0, 2.f);
            REQUIRE(Utility::setBestTime(save, 0, 1.f));
        }
    }
}
