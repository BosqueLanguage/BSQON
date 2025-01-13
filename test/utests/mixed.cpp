#include <boost/test/unit_test.hpp>

#include "../test_driver.h"

#define TEST_IT(TYPE, BSQ) { std::u8string result; std::u8string expected; tround(createMetaPathName("mixed"), TYPE, createBSQONPathName("mixed", BSQ), expected, result); checkAndReport(result, expected); }
#define TEST_WITH(TYPE, BSQ, EXPECTED) { std::u8string result; std::u8string orig; tround(createMetaPathName("mixed"), TYPE, createBSQONPathName("mixed", BSQ), orig, result); checkAndReport(result, EXPECTED); }

BOOST_AUTO_TEST_SUITE(Mixed)

std::u8string s_binop1 = u8"Main::Add{Main::Value{1i}, Main::Value{2i}}";
std::u8string s_binop2 = u8"Main::Sub{Main::Value{2i}, Main::Value{1i}}";
std::u8string s_binop3 = u8"Main::Add{Main::Sub{Main::Value{3i}, Main::Value{2i}}, Main::Value{2i}}";

////
//BaseOps
BOOST_AUTO_TEST_SUITE(BaseOps)
BOOST_AUTO_TEST_CASE(value_direct) {
    TEST_IT("Main::Value", "mixed_value.bsqon");
}
BOOST_AUTO_TEST_CASE(value_general) {
    TEST_IT("Main::Expression", "mixed_value.bsqon");
}
BOOST_AUTO_TEST_SUITE_END() //BaseOps

////
//BinOp
BOOST_AUTO_TEST_SUITE(BinOp)
BOOST_AUTO_TEST_CASE(add_direct) {
    TEST_WITH("Main::Add", "mixed_add.bsqon", s_binop1);
}
BOOST_AUTO_TEST_CASE(add_general) {
    TEST_WITH("Main::Expression", "mixed_add.bsqon", s_binop1);
}
BOOST_AUTO_TEST_CASE(sub_general) {
    TEST_WITH("Main::Expression", "mixed_sub.bsqon", s_binop2);
}
BOOST_AUTO_TEST_CASE(nested) {
    TEST_WITH("Main::Expression", "mixed_nested.bsqon", s_binop3);
}
BOOST_AUTO_TEST_SUITE_END() //BinOp

BOOST_AUTO_TEST_SUITE_END() //Mixed
