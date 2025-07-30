#include <boost/test/unit_test.hpp>

#include "../smt_test_driver.h"

#define TEST_IT(SMT, FN_SIG, BSQON, MODE, EXPECT)                                                                      \
    {                                                                                                                  \
        std::u8string result;                                                                                          \
        std::u8string expected = EXPECT;                                                                               \
        smt_tround(createSmtPathName(SMT), createSmtJSONPathName(FN_SIG), createSmtJSONPathName(BSQON), MODE, result); \
        checkAndReport(result, expected);                                                                              \
    }

BOOST_AUTO_TEST_SUITE(Concept)

BOOST_AUTO_TEST_CASE(concept_list)
{
    TEST_IT("concepts/list_prim", "concepts/list_prim_fn", "concepts/list_prim", "--extract", u8"List<Int>{10i}");
}

BOOST_AUTO_TEST_CASE(option_some)
{
    TEST_IT("concepts/concept_option_some", "concepts/concept_option_some_fn", "concepts/concept_option_some",
            "--extract", u8"Some<Int>{3i}");
}

BOOST_AUTO_TEST_CASE(option_none)
{
    TEST_IT("concepts/concept_option_none", "concepts/concept_option_none_fn", "concepts/concept_option_none",
            "--extract", u8"none");
}

BOOST_AUTO_TEST_CASE(Optional_CRegexes_entity_invariants)
{
    TEST_IT("concepts/CustomerRestController", "concepts/CustomerRestController_fn", "concepts/CustomerRestController",
            "--extract",
            u8"Main::UserDTO{Some<Main::EMAIL>{'jch270@uky.edu'<Main::EMAIL>}, '12345678'<Main::ALPHANUMERIC>, "
            u8"Some<Main::USER_NAME>{'James Chen'<Main::USER_NAME>}, Some<Main::USER_PHONE>{'+12 "
            u8"111-111-1111'<Main::USER_PHONE>}, Some<Main::USER_ADDRESS>{'123 Fake Street'<Main::USER_ADDRESS>}}");
}

BOOST_AUTO_TEST_SUITE_END() // DataType
