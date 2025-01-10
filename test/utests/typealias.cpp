#include <boost/test/unit_test.hpp>

#include "../test_driver.h"

#define TEST_IT(TYPE, BSQ) { std::u8string result; std::u8string expected; tround(createMetaPathName("typealias"), TYPE, createBSQONPathName("typealias", BSQ), expected, result); checkAndReport(result, expected); }

BOOST_AUTO_TEST_SUITE(TypeAlias)

////
//TypeAlias
BOOST_AUTO_TEST_SUITE(TypeAlias_Direct)
BOOST_AUTO_TEST_CASE(fahrenheit) {
    TEST_IT("Main::Fahrenheit", "fahrenheit.bsqon");
}
BOOST_AUTO_TEST_CASE(cid) {
    TEST_IT("Main::CID", "cid.bsqon");
}
BOOST_AUTO_TEST_SUITE_END() //TypeAlias

////
//TypeAlias with Validate
BOOST_AUTO_TEST_SUITE(TypeAlias_WValidate)
BOOST_AUTO_TEST_CASE(sid) {
    TEST_IT("Main::SID", "sid.bsqon");
}
BOOST_AUTO_TEST_SUITE_END() //TypeAlias with Validate

BOOST_AUTO_TEST_SUITE_END() //TypeAlias
