#include <boost/test/unit_test.hpp>

#include "../smt_test_driver.h"

#define TEST_IT(SMT, FN_SIG, BSQON, MODE, EXPECT)                                                                      \
    {                                                                                                                  \
        std::u8string result;                                                                                          \
        std::u8string expected = EXPECT;                                                                               \
        smt_tround(createSmtPathName(SMT), createSmtJSONPathName(FN_SIG), createSmtJSONPathName(BSQON), MODE, result); \
        checkAndReport(result, expected);                                                                              \
    }

BOOST_AUTO_TEST_SUITE(DataType)

BOOST_AUTO_TEST_CASE(int_entity)
{
    TEST_IT("datatype/dt_int", "datatype/dt_int_fn", "datatype/dt_int", "--extract", u8"Main::Foo{0i, 0i}");
}

BOOST_AUTO_TEST_CASE(pixel_entity)
{
    TEST_IT("datatype/dt_pixel", "datatype/dt_pixel_fn", "datatype/dt_pixel", "--extract",
            u8"Main::Pixel{250i, 0i, 120i, 245i}");
}

BOOST_AUTO_TEST_CASE(primitives_entity)
{
    TEST_IT("datatype/dt_prim", "datatype/dt_prim_fn", "datatype/dt_prim", "--extract",
            u8"Main::Prim{-52i, 77N, 'Sphinx of black quartz, judge my vow.', 'Mr. Jock, TV quiz PhD, bags few lynx.', "
            u8"false, true}");
}

BOOST_AUTO_TEST_CASE(Optional_CRegexes_entity)
{
    TEST_IT("datatype/CustomerRegex", "datatype/CustomerRegex_fn", "datatype/CustomerRegex", "--extract",
            u8"Main::UserDTO{Some<Main::EMAIL>{'jch270@uky.edu'<Main::EMAIL>}, '12345678'<Main::ALPHANUMERIC>, "
            u8"Some<Main::USER_NAME>{'James Chen'<Main::USER_NAME>}, Some<Main::USER_PHONE>{'+12 "
            u8"111-111-1111'<Main::USER_PHONE>}, Some<Main::USER_ADDRESS>{'123 Fake Street'<Main::USER_ADDRESS>}}");
}

BOOST_AUTO_TEST_SUITE_END() // DataType
