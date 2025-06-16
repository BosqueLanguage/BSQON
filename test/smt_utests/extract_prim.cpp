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
    TEST_IT("--Main::Man", "prim_CString", "prim_CString", u8"Main::Man{'Manchester'}");
}

BOOST_AUTO_TEST_CASE(Bool)
{
    TEST_IT("--Main::Boo", "prim_Bool", "prim_Bool", u8"Main::Boo{true, false, true, true, false}");
}

BOOST_AUTO_TEST_CASE(IntNums)
{
    TEST_IT("--Main::Nums", "prim_Int", "prim_Int", u8"Main::Nums{55i, -22i, 375I, -288I}");
}

BOOST_AUTO_TEST_CASE(NatNums)
{
    TEST_IT("--Main::Nats", "prim_Nat", "prim_Nat", u8"Main::Nats{55n, 0n, 375N, 222N}");
}

BOOST_AUTO_TEST_SUITE_END() // Primitives
