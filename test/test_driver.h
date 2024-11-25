#include "../src/info/type_info.h"
#include "../src/info/bsqon.h"
#include "../src/parser/bsqon_parse.h"

void checkAndReport(const std::u8string& result, const std::u8string& expected);

std::string createMetaPathName(std::string tcc);
std::string createBSQONPathName(std::string tcc, std::string bsq);

void tround(std::string metafile, const char* type, std::string datafile, std::u8string& contents, std::u8string& result);
