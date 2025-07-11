#include <boost/test/unit_test.hpp>

#include "../smt_test_driver.h"

#define TEST_IT(SMT, FN_SIG, BSQON, EXPECT)                                                                            \
    {                                                                                                                  \
        std::u8string result;                                                                                          \
        std::u8string expected = EXPECT;                                                                               \
        smt_tround(createSmtPathName(SMT), createSmtJSONPathName(FN_SIG), createSmtJSONPathName(BSQON), result);       \
        checkAndReport(result, expected);                                                                              \
    }

BOOST_AUTO_TEST_SUITE(Concept)

BOOST_AUTO_TEST_CASE(concept_list)
{
    TEST_IT("list_prim", "list_prim_fn", "list_prim", u8"List<Int>{10i, 2i, 5i}\n");
}

BOOST_AUTO_TEST_SUITE_END() // DataType
