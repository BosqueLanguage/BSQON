#include <boost/test/unit_test.hpp>

#include "../smt_test_driver.h"

#define TEST_IT(TYPE, SMT, BSQON, EXPECT)                                                                              \
    {                                                                                                                  \
        std::u8string result;                                                                                          \
        std::u8string expected = EXPECT;                                                                               \
        smt_tround(createSmtPathName(SMT), createSmtBSQONPathName(BSQON), TYPE, result);                               \
        checkAndReport(result, expected);                                                                              \
    }

BOOST_AUTO_TEST_SUITE(DataType)

BOOST_AUTO_TEST_CASE(int_entity)
{
    TEST_IT("--Main::Foo", "dt_int", "dt_int", u8"Main::Foo{0i, 0i}");
}
// BOOST_AUTO_TEST_CASE(Cstr_entity)
// {
//     TEST_IT("--Main::Bar", "smt_dt_Cstr", "smt_dt_Cstr", u8"Main::Bar{Manchester}");
// }

BOOST_AUTO_TEST_SUITE_END() // DataType
