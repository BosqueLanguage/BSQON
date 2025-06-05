#include <boost/test/unit_test.hpp>

#include "../test_driver.h"

#define TEST_IT(TYPE, SMT, BSQ, EXPECT)                                                                                \
    {                                                                                                                  \
        std::u8string result;                                                                                          \
        std::u8string expected = EXPECT;                                                                               \
        smt_tround(createSmtPathName("smt_dt"), createSmtBSQONPathName("smt_dt"), TYPE, result);                       \
        checkAndReport(result, expected);                                                                              \
    }

BOOST_AUTO_TEST_SUITE(DataType)

BOOST_AUTO_TEST_CASE(int_entity)
{
    TEST_IT("--Main::Foo", "smt_dt.smt2", "smt_dt.json", u8"Main::Foo{0i, 0i}");
}

BOOST_AUTO_TEST_SUITE_END() // DataType
