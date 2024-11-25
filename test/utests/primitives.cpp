#include <boost/test/unit_test.hpp>

#include "../test_driver.h"

#define TEST_PRIMITIVE(TY, BSQ) { std::u8string result; std::u8string expected; tround(createMetaPathName("primitives"), TY, createBSQONPathName("primitives", BSQ), expected, result); checkAndReport(result, expected); }

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

////
//BigNat
BOOST_AUTO_TEST_SUITE(BigNat)
BOOST_AUTO_TEST_CASE(bignat0) {
    TEST_PRIMITIVE("BigNat", "bignat_0.bsqon");
}
BOOST_AUTO_TEST_CASE(bignat3) {
    TEST_PRIMITIVE("BigNat", "bignat_3.bsqon");
}
BOOST_AUTO_TEST_SUITE_END() //BigNat

////
//BigInt
BOOST_AUTO_TEST_SUITE(BigInt)
BOOST_AUTO_TEST_CASE(bigint0) {
    TEST_PRIMITIVE("BigInt", "bigint_0.bsqon");
}
BOOST_AUTO_TEST_CASE(bigintn3) {
    TEST_PRIMITIVE("BigInt", "bigint_-3.bsqon");
}
BOOST_AUTO_TEST_CASE(bigint3) {
    TEST_PRIMITIVE("BigInt", "bigint_3.bsqon");
}
BOOST_AUTO_TEST_SUITE_END() //BigInt

////
//Float
BOOST_AUTO_TEST_SUITE(Float)
BOOST_AUTO_TEST_CASE(float1) {
    TEST_PRIMITIVE("Float", "float_1.bsqon");
}
BOOST_AUTO_TEST_CASE(floatn3) {
    TEST_PRIMITIVE("Float", "float_-3.bsqon");
}
BOOST_AUTO_TEST_CASE(floatpt3) {
    TEST_PRIMITIVE("Float", "float_.3.bsqon");
}
BOOST_AUTO_TEST_SUITE_END() //Float

BOOST_AUTO_TEST_SUITE_END() //Primitives
