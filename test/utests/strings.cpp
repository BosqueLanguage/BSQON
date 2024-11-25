#include <boost/test/unit_test.hpp>

#include "../test_driver.h"

#define TEST_STRING(BSQ) { std::u8string result; std::u8string expected; tround(createMetaPathName("primitives"), "String", createBSQONPathName("strings", BSQ), expected, result); checkAndReport(result, expected); }

#define TEST_CSTRING(BSQ) { std::u8string result; std::u8string expected; tround(createMetaPathName("primitives"), "CString", createBSQONPathName("strings", BSQ), expected, result); checkAndReport(result, expected); }


BOOST_AUTO_TEST_SUITE(Strings)

////
//String
BOOST_AUTO_TEST_SUITE(UnicodeStrings)
BOOST_AUTO_TEST_CASE(empty) {
    TEST_STRING("string_empty.bsqon");
}
BOOST_AUTO_TEST_CASE(hello) {
    TEST_STRING("string_hello.bsqon");
}
BOOST_AUTO_TEST_CASE(none) {
    TEST_STRING("string_none.bsqon");
}
BOOST_AUTO_TEST_CASE(percent) {
    TEST_STRING("string_percent.bsqon");
}
BOOST_AUTO_TEST_CASE(quote) {
    TEST_STRING("string_quote.bsqon");
}
BOOST_AUTO_TEST_CASE(unicode) {
    TEST_STRING("string_unicode.bsqon");
}
BOOST_AUTO_TEST_SUITE_END() //String

////
//CString
BOOST_AUTO_TEST_SUITE(CharStrings)
BOOST_AUTO_TEST_CASE(empty) {
    TEST_CSTRING("cstring_empty.bsqon");
}
BOOST_AUTO_TEST_CASE(hello) {
    TEST_CSTRING("cstring_hello.bsqon");
}
BOOST_AUTO_TEST_CASE(none) {
    TEST_CSTRING("cstring_none.bsqon");
}
BOOST_AUTO_TEST_CASE(percent) {
    TEST_CSTRING("cstring_percent.bsqon");
}
BOOST_AUTO_TEST_CASE(quote) {
    TEST_CSTRING("cstring_quote.bsqon");
}
BOOST_AUTO_TEST_CASE(unicode) {
    TEST_CSTRING("cstring_unicode.bsqon");
}
BOOST_AUTO_TEST_SUITE_END() //CString

BOOST_AUTO_TEST_SUITE_END() //Strings
