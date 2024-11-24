#include <boost/test/unit_test.hpp>

void checkAndReport(const std::u8string& result, const std::u8string& expected) {
    if(result != expected) {
        std::cout << "Expected: " << std::string(expected.cbegin(), expected.cend()) << std::endl;
        std::cout << "Got: " << std::string(result.cbegin(), result.cend()) << std::endl;
    }

    BOOST_ASSERT(result == expected);
}

void mprint()
{
    std::cout << "Hello, World -- " << TEST_PATH << std::endl;
}

//#define TEST_PRIMITIVE(TY, BSQ) { std::string meta = std::string(TEST_PATH) + "/asm_meta/primitives.json"; std::string bsq = std::string(TEST_PATH) + "/bsqon/primitives/" + BSQ; std::u8string& result; std::u8string& expected; tround(meta.c_str(), TY, bsq, expected, result); checkAndReport(result, expected); }
#define TEST_PRIMITIVE(TY, BSQ) { mprint(); }

BOOST_AUTO_TEST_SUITE(Primitives)

////
//None
BOOST_AUTO_TEST_SUITE(None)
BOOST_AUTO_TEST_CASE(none) {
    TEST_PRIMITIVE("None", "none.bsqon");
}
BOOST_AUTO_TEST_SUITE_END() //None

////
//Bool
BOOST_AUTO_TEST_SUITE(Bool)
BOOST_AUTO_TEST_CASE(ttrue) {
    TEST_PRIMITIVE("Bool", "bool_true.bsqon");
}
BOOST_AUTO_TEST_CASE(tfalse) {
    TEST_PRIMITIVE("Bool", "bool_false.bsqon");
}
BOOST_AUTO_TEST_SUITE_END() //Bool

BOOST_AUTO_TEST_SUITE_END() //Primitives
