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

BOOST_AUTO_TEST_CASE(pixel_entity)
{
    TEST_IT("--Main::Pixel", "dt_pixel", "dt_pixel", u8"Main::Pixel{250i, 0i, 120i, 245i}");
}

BOOST_AUTO_TEST_CASE(primitives_entity)
{
    TEST_IT("--Main::Prim", "dt_prim", "dt_prim", u8"Main::Prim{-52i,77N,\'Scribble\',\'Scribble2\',false,true}");
}

BOOST_AUTO_TEST_SUITE_END() // DataType
