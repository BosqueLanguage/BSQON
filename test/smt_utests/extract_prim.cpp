#include <boost/test/unit_test.hpp>

#include "../smt_test_driver.h"

#define TEST_IT(SMT, FN_SIG, BSQON, MODE, EXPECT)                                                                      \
    {                                                                                                                  \
        std::u8string result;                                                                                          \
        std::u8string expected = EXPECT;                                                                               \
        smt_tround(createSmtPathName(SMT), createSmtJSONPathName(FN_SIG), createSmtJSONPathName(BSQON), MODE, result); \
        checkAndReport(result, expected);                                                                              \
    }

BOOST_AUTO_TEST_SUITE(Extract_Primitives)

BOOST_AUTO_TEST_CASE(Exract_CString)
{
    TEST_IT("primitives/prim_CString", "primitives/prim_CString_fn", "primitives/prim_CString", "--extract",
            u8"'Manchester'");
}

BOOST_AUTO_TEST_CASE(Extract_Bool)
{
    TEST_IT("primitives/prim_Bool", "primitives/prim_Bool_fn", "primitives/prim_Bool", "--extract", u8"false");
}

BOOST_AUTO_TEST_CASE(Extract_Int)
{
    TEST_IT("primitives/prim_Int", "primitives/prim_Int_fn", "primitives/prim_Int", "--extract", u8"13I31i");
}

BOOST_AUTO_TEST_CASE(Extract_Nat)
{
    TEST_IT("primitives/prim_Nat", "primitives/prim_Nat_fn", "primitives/prim_Nat", "--extract", u8"92n");
}

BOOST_AUTO_TEST_SUITE_END()
