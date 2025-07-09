#include <boost/test/unit_test.hpp>

#include "../smt_test_driver.h"

#define TEST_IT(SMT, FN_SIG, BSQON, EXPECT)                                                                            \
    {                                                                                                                  \
        std::u8string result;                                                                                          \
        std::u8string expected = EXPECT;                                                                               \
        smt_tround(createSmtPathName(SMT), createSmtJSONPathName(FN_SIG), createSmtJSONPathName(BSQON), result);       \
        checkAndReport(result, expected);                                                                              \
    }

BOOST_AUTO_TEST_SUITE(DataType)

BOOST_AUTO_TEST_CASE(int_entity)
{
    TEST_IT("datatype/dt_int", "datatype/dt_int_fn", "datatype/dt_int", u8"Main::Foo{0i, 0i}\n");
}

BOOST_AUTO_TEST_CASE(pixel_entity)
{
    TEST_IT("datatype/dt_pixel", "datatype/dt_pixel_fn", "datatype/dt_pixel", u8"Main::Pixel{250i, 0i, 120i, 245i}\n");
}

BOOST_AUTO_TEST_CASE(primitives_entity)
{
    TEST_IT("datatype/dt_prim", "datatype/dt_prim_fn", "datatype/dt_prim",
            u8"Main::Prim{-52i, 77N, 'Sphinx of black quartz, judge my vow.', 'Mr. Jock, TV quiz PhD, bags few lynx.', "
            u8"false, true}\n");
}

BOOST_AUTO_TEST_SUITE_END() // DataType
