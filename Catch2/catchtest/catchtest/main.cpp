#define CATCH_CONFIG_MAIN
#include <catch.hpp>

int foo() {
    return 3;
}

TEST_CASE("A test test case", "[mytest]") {
    int x = foo();
    REQUIRE(x == 3);
}