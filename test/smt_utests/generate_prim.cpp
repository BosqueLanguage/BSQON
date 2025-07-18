#include <boost/test/unit_test.hpp>

#include "../smt_test_driver.h"

#define TEST_IT(SMT, FN_SIG, BSQON, EXPECT)                                                                            \
    {                                                                                                                  \
        std::u8string result;                                                                                          \
        std::u8string expected = EXPECT;                                                                               \
        smt_tround(createSmtPathName(SMT), createSmtJSONPathName(FN_SIG), createSmtJSONPathName(BSQON), result);       \
        checkAndReport(result, expected);                                                                              \
    }

BOOST_AUTO_TEST_SUITE(Generate_Primitives)

BOOST_AUTO_TEST_CASE(Generate_Int)
{
    TEST_IT("primitives/prim_Int", "primitives/prim_Int_fn", "primitives/prim_Int", u8"");
}

BOOST_AUTO_TEST_SUITE_END() // Primitives
