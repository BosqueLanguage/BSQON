#include <boost/test/unit_test.hpp>

#include "../smt_test_driver.h"

#define TEST_IT(SMT, FN_SIG, BSQON, EXPECT)                                                                            \
    {                                                                                                                  \
        std::u8string result;                                                                                          \
        std::u8string expected = EXPECT;                                                                               \
        smt_tround(createSmtPathName(SMT), createSmtJSONPathName(FN_SIG), createSmtJSONPathName(BSQON), result);       \
        checkAndReport(result, expected);                                                                              \
    }

BOOST_AUTO_TEST_SUITE(Primitives)

BOOST_AUTO_TEST_CASE(CString)
{
    TEST_IT("prim_CString", "prim_CString_fn", "prim_CString", u8"'Manchester'\n");
}

BOOST_AUTO_TEST_CASE(Bool)
{
    TEST_IT("prim_Bool", "prim_Bool_fn", "prim_Bool", u8"false\n");
}

BOOST_AUTO_TEST_CASE(Ints)
{
    TEST_IT("prim_Int", "prim_Int_fn", "prim_Int", u8"457I\n33i\n");
}

BOOST_AUTO_TEST_CASE(Nat)
{
    TEST_IT("prim_Nat", "prim_Nat_fn", "prim_Nat", u8"92n\n");
}

BOOST_AUTO_TEST_SUITE_END() // Primitives
