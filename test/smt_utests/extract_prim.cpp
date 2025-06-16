#include <boost/test/unit_test.hpp>

#include "../smt_test_driver.h"

#define TEST_IT(TYPE, SMT, BSQON, EXPECT)                                                                              \
    {                                                                                                                  \
        std::u8string result;                                                                                          \
        std::u8string expected = EXPECT;                                                                               \
        smt_tround(createSmtPathName(SMT), createSmtBSQONPathName(BSQON), TYPE, result);                               \
        checkAndReport(result, expected);                                                                              \
    }

BOOST_AUTO_TEST_SUITE(Primitives)

BOOST_AUTO_TEST_CASE(CString)
{
    TEST_IT("--CString", "prim_CString", "prim_CString", u8"'Manchester'");
}

BOOST_AUTO_TEST_CASE(Bool)
{
    TEST_IT("--Bool", "prim_Bool", "prim_Bool", u8"false");
}

BOOST_AUTO_TEST_CASE(Int)
{
    TEST_IT("--Int", "prim_Int", "prim_Int", u8"33i");
}

// BOOST_AUTO_TEST_CASE(BigInt)
// {
//     TEST_IT("--BigInt", "prim_Int", "prim_Int", u8"33I");
// }

BOOST_AUTO_TEST_CASE(Nat)
{
    TEST_IT("--Nat", "prim_Nat", "prim_Nat", u8"92n");
}

// BOOST_AUTO_TEST_CASE(BigNat)
// {
//     TEST_IT("--BigNat", "prim_Nat", "prim_Nat", u8"0N");
// }

BOOST_AUTO_TEST_SUITE_END() // Primitives
