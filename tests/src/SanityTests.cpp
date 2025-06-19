#include "Helper.hpp"
#include <catch_amalgamated.hpp>

TEST_CASE("[Sanity]")
{
    SECTION("Can create dummy window and dependencies")
    {
        auto window = Helper::createDummyWindow();
        auto dic = Helper::createDummyDependencies(window);
        REQUIRE(true);
    }
}
