#include "../src/info/type_info.h"
#include "../src/info/bsqon.h"
#include "../src/parser/bsqon_parse.h"
#include "../src/smtextract/smt_utils.h"
#include "../src/smtextract/smt_extract.h"

void checkAndReport(const std::u8string& result, const std::u8string& expected);

std::string createSmtPathName(std::string tcc);
std::string createSmtBSQONPathName(std::string tcc);

void smt_tround(std::string smt_in, std::string meta_file, const char* t_in, std::u8string& result);
