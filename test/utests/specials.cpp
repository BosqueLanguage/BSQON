#include <boost/test/unit_test.hpp>

#include "../test_driver.h"

#define TEST_IT(TYPE, BSQ) { std::u8string result; std::u8string expected; tround(createMetaPathName("specials"), TYPE, createBSQONPathName("specials", BSQ), expected, result); checkAndReport(result, expected); }

BOOST_AUTO_TEST_SUITE(Specials)

////
//Option
BOOST_AUTO_TEST_SUITE(Option)
BOOST_AUTO_TEST_CASE(none) {
    TEST_IT("None", "none_direct.bsqon");
}
BOOST_AUTO_TEST_CASE(none_opt) {
    TEST_IT("Option<Int>", "none_option.bsqon");
}
BOOST_AUTO_TEST_CASE(some) {
    TEST_IT("Some<Int>", "some_direct.bsqon");
}
BOOST_AUTO_TEST_CASE(some_opt) {
    TEST_IT("Option<Int>", "some_option.bsqon");
}
BOOST_AUTO_TEST_SUITE_END() //Option

////
//Result
BOOST_AUTO_TEST_SUITE(Result)
BOOST_AUTO_TEST_CASE(ok) {
    TEST_IT("Result<Int, Bool>", "result_ok.bsqon");
}
BOOST_AUTO_TEST_CASE(fail) {
    TEST_IT("Result<Int, Bool>", "result_err.bsqon");
}
BOOST_AUTO_TEST_SUITE_END() //Result

BOOST_AUTO_TEST_SUITE_END() //Specials
