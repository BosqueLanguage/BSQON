#include <boost/test/unit_test.hpp>

#include "../test_driver.h"

#define TEST_IT(TYPE, BSQ) { std::u8string result; std::u8string expected; tround(createMetaPathName("collections"), TYPE, createBSQONPathName("collections", BSQ), expected, result); checkAndReport(result, expected); }

BOOST_AUTO_TEST_SUITE(Collections)

////
//Empty
BOOST_AUTO_TEST_SUITE(Empty)
BOOST_AUTO_TEST_CASE(list_empty) {
    TEST_IT("List<Int>", "list_empty.bsqon");
}
BOOST_AUTO_TEST_CASE(map_empty) {
    TEST_IT("Map<Int, Bool>", "map_empty.bsqon");
}
BOOST_AUTO_TEST_SUITE_END() //Empty

////
//Data
BOOST_AUTO_TEST_SUITE(Data)
BOOST_AUTO_TEST_CASE(list_data) {
    TEST_IT("List<Int>", "list_3.bsqon");
}
BOOST_AUTO_TEST_CASE(map_data) {
    TEST_IT("Map<Int, Bool>", "map_2.bsqon");
}
BOOST_AUTO_TEST_SUITE_END() //Data

BOOST_AUTO_TEST_SUITE_END() //Collections
