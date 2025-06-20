#include <boost/test/unit_test.hpp>

#include "../test_driver.h"

#define TEST_IT(TYPE, BSQ)                                                                                             \
    {                                                                                                                  \
        std::u8string result;                                                                                          \
        std::u8string expected;                                                                                        \
        tround(createMetaPathName("datatype"), TYPE, createBSQONPathName("datatype", BSQ), expected, result);          \
        checkAndReport(result, expected);                                                                              \
    }

BOOST_AUTO_TEST_SUITE(DataType)

////
// BaseOps
BOOST_AUTO_TEST_SUITE(BaseOps)
BOOST_AUTO_TEST_CASE(bar_direct)
{
    TEST_IT("Main::Bar", "datatype_bar.bsqon");
}
BOOST_AUTO_TEST_CASE(baz_direct)
{
    TEST_IT("Main::Baz", "datatype_baz.bsqon");
}
BOOST_AUTO_TEST_SUITE_END() // BaseOps

////
// Foo
BOOST_AUTO_TEST_SUITE(Foo)
BOOST_AUTO_TEST_CASE(bar_general)
{
    TEST_IT("Main::Foo", "datatype_bar.bsqon");
}
BOOST_AUTO_TEST_CASE(baz_general)
{
    TEST_IT("Main::Foo", "datatype_baz.bsqon");
}
BOOST_AUTO_TEST_SUITE_END() // Foo

BOOST_AUTO_TEST_SUITE_END() // DataType
