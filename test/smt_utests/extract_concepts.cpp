#include <boost/test/unit_test.hpp>

#include "../smt_test_driver.h"

#define TEST_IT(SMT, FN_SIG, BSQON, MODE, EXPECT)                                                                      \
    {                                                                                                                  \
        std::u8string result;                                                                                          \
        std::u8string expected = EXPECT;                                                                               \
        smt_tround(createSmtPathName(SMT), createSmtJSONPathName(FN_SIG), createSmtJSONPathName(BSQON), MODE, result); \
        checkAndReport(result, expected);                                                                              \
    }

BOOST_AUTO_TEST_SUITE(Extract_Concepts)

BOOST_AUTO_TEST_CASE(option_some)
{
    TEST_IT("concepts/option_some", "concepts/option_some_fn", "concepts/option_some", "--extract", u8"Some<Int>{3i}");
}

BOOST_AUTO_TEST_CASE(option_none)
{
    TEST_IT("concepts/option_none", "concepts/option_none_fn", "concepts/option_none", "--extract", u8"none");
}

BOOST_AUTO_TEST_CASE(Optional_CRegexes_entity_invariants)
{
    TEST_IT("concepts/CustomerRestController", "concepts/CustomerRestController_fn", "concepts/CustomerRestController",
            "--extract",
            u8"Main::UserDTO{Some<Main::EMAIL>{'jch270@uky.edu'<Main::EMAIL>}, '12345678'<Main::ALPHANUMERIC>, "
            u8"Some<Main::USER_NAME>{'James Chen'<Main::USER_NAME>}, Some<Main::USER_PHONE>{'+12 "
            u8"111-111-1111'<Main::USER_PHONE>}, Some<Main::USER_ADDRESS>{'123 Fake Street'<Main::USER_ADDRESS>}}");
}

BOOST_AUTO_TEST_CASE(List_Length_0)
{
    TEST_IT("concepts/list_0_prim", "concepts/list_0_prim_fn", "concepts/list_0_prim", "--extract", u8"List<Int>{}");
}

BOOST_AUTO_TEST_CASE(List_Length_1)
{
    TEST_IT("concepts/list_1_prim", "concepts/list_1_prim_fn", "concepts/list_1_prim", "--extract", u8"List<Int>{27i}");
}

BOOST_AUTO_TEST_CASE(List_Length_2)
{
    TEST_IT("concepts/list_2_prim", "concepts/list_2_prim_fn", "concepts/list_2_prim", "--extract",
            u8"List<Int>{8i, 7i}");
}

BOOST_AUTO_TEST_CASE(List_Length_3)
{
    TEST_IT("concepts/list_3_prim", "concepts/list_3_prim_fn", "concepts/list_3_prim", "--extract",
            u8"List<Int>{8i, 7i, 6i}");
}

BOOST_AUTO_TEST_SUITE_END() // DataType
