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
    TEST_IT("primitives/prim_CString", "primitives/prim_CString_fn", "primitives/prim_CString", u8"'Manchester'\n");
}

BOOST_AUTO_TEST_CASE(Bool)
{
    TEST_IT("primitives/prim_Bool", "primitives/prim_Bool_fn", "primitives/prim_Bool", u8"false\n");
}

BOOST_AUTO_TEST_CASE(Ints)
{
    TEST_IT("primitives/prim_Int", "primitives/prim_Int_fn", "primitives/prim_Int", u8"457I\n33i\n");
}

BOOST_AUTO_TEST_CASE(Nat)
{
    TEST_IT("primitives/prim_Nat", "primitives/prim_Nat_fn", "primitives/prim_Nat", u8"92n\n");
}

BOOST_AUTO_TEST_SUITE_END() // Primitives
