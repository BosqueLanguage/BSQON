#include <boost/test/unit_test.hpp>

#include "../smt_test_driver.h"

#define TEST_IT(SMT, FN_SIG, BSQON, MODE, EXPECT)                                                                      \
    {                                                                                                                  \
        std::u8string result;                                                                                          \
        std::u8string expected = EXPECT;                                                                               \
        smt_tround(createSmtPathName(SMT), createSmtJSONPathName(FN_SIG), createSmtJSONPathName(BSQON), MODE, result); \
        checkAndReport(result, expected);                                                                              \
    }

BOOST_AUTO_TEST_SUITE(Generate_Concepts)

BOOST_AUTO_TEST_CASE(List_Length_0)
{
    TEST_IT("concepts/list_0_prim", "concepts/list_0_prim_fn", "concepts/list_0_prim", "--generate",
            u8"(define-fun f () List<Int> (List<Int>-mk(@Term-ListOps@Vector0<Int>-mk(ListOps@Vector0<Int>-mk ))))");
}

BOOST_AUTO_TEST_CASE(List_Length_1)
{
    TEST_IT("concepts/list_1_prim", "concepts/list_1_prim_fn", "concepts/list_1_prim", "--generate",
            u8"(define-fun f () List<Int> (List<Int>-mk(@Term-ListOps@Vector1<Int>-mk(ListOps@Vector1<Int>-mk 27))))");
}

BOOST_AUTO_TEST_CASE(List_Length_2)
{
    TEST_IT("concepts/list_2_prim", "concepts/list_2_prim_fn", "concepts/list_2_prim", "--generate",
            u8"(define-fun f () List<Int> (List<Int>-mk(@Term-ListOps@Vector2<Int>-mk(ListOps@Vector2<Int>-mk 8 "
            "7))))");
}

BOOST_AUTO_TEST_CASE(List_Length_3)
{
    TEST_IT("concepts/list_3_prim", "concepts/list_3_prim_fn", "concepts/list_3_prim", "--generate",
            u8"(define-fun f () List<Int> (List<Int>-mk(@Term-ListOps@Vector3<Int>-mk(ListOps@Vector3<Int>-mk 8 "
            "7 6))))");
}

BOOST_AUTO_TEST_SUITE_END() // Primitives
