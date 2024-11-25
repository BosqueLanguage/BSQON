#include <boost/test/unit_test.hpp>

#include "../test_driver.h"

void checkAndReport(const std::u8string& result, const std::u8string& expected) {
    if(result != expected) {
        std::cout << "Expected: " << std::string(expected.cbegin(), expected.cend()) << std::endl;
        std::cout << "Got: " << std::string(result.cbegin(), result.cend()) << std::endl;
    }

    BOOST_ASSERT(result == expected);
}

#ifndef TEST_PATH
#define TEST_PATH "./"
#endif

std::string createMetaPathName()
{
    return std::string(TEST_PATH) + "/asm_meta/primitives.json";
}

std::string createBSQONPathName(std::string bsq)
{
    return std::string(TEST_PATH) + "/bsqon/primitives/" + bsq;
}

#define TEST_PRIMITIVE(TY, BSQ) { std::u8string result; std::u8string expected; tround(createMetaPathName(), TY, createBSQONPathName(BSQ), expected, result); checkAndReport(result, expected); }

BOOST_AUTO_TEST_SUITE(Primitives)

////
//None
BOOST_AUTO_TEST_SUITE(None)
BOOST_AUTO_TEST_CASE(none) {
    TEST_PRIMITIVE("None", "none.bsqon");
}
BOOST_AUTO_TEST_SUITE_END() //None

////
//Bool
BOOST_AUTO_TEST_SUITE(Bool)
BOOST_AUTO_TEST_CASE(ttrue) {
    TEST_PRIMITIVE("Bool", "bool_true.bsqon");
}
BOOST_AUTO_TEST_CASE(tfalse) {
    TEST_PRIMITIVE("Bool", "bool_false.bsqon");
}
BOOST_AUTO_TEST_SUITE_END() //Bool

////
//Nat
BOOST_AUTO_TEST_SUITE(Nat)
BOOST_AUTO_TEST_CASE(nat0) {
    TEST_PRIMITIVE("Nat", "nat_0.bsqon");
}
BOOST_AUTO_TEST_CASE(nat3) {
    TEST_PRIMITIVE("Nat", "nat_3.bsqon");
}
BOOST_AUTO_TEST_SUITE_END() //Nat

////
//Int
BOOST_AUTO_TEST_SUITE(Int)
BOOST_AUTO_TEST_CASE(int0) {
    TEST_PRIMITIVE("Int", "int_0.bsqon");
}
BOOST_AUTO_TEST_CASE(intn3) {
    TEST_PRIMITIVE("Int", "int_-3.bsqon");
}
BOOST_AUTO_TEST_CASE(int3) {
    TEST_PRIMITIVE("Int", "int_3.bsqon");
}
BOOST_AUTO_TEST_SUITE_END() //Int

BOOST_AUTO_TEST_SUITE_END() //Primitives
