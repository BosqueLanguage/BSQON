MAKE_PATH=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
BUILD_DIR=$(MAKE_PATH)/
BIN_DIR=$(MAKE_PATH)/../bin/
SRC_DIR=$(MAKE_PATH)/../src/

PARSER_GEN_DIR=$(SRC_DIR)fbparser/
INFO_DIR=$(SRC_DIR)info/
PARSER_DIR=$(SRC_DIR)parser/
GEN_DIR=$(SRC_DIR)testgen/
SMT_DIR=$(SRC_DIR)smtextract/

OUT_EXE=$(BUILD_DIR)output/
OUT_OBJ=$(BUILD_DIR)output/obj/
PCKG_DIR=$(BUILD_DIR)bsqon/

JSON_INCLUDES=-I $(BUILD_DIR)include/json/
BREX_INCLUDES=-I $(BUILD_DIR)include/brex/
BREX_PATH=$(BUILD_DIR)include/brex/
Z3_INCLUDES=-I $(BUILD_DIR)include/z3/
Z3_PATH=$(BUILD_DIR)include/z3/
LIB_PATH=$(OUT_EXE)

SMT_TEST_DIR=$(MAKE_PATH)/../test/
SMT_TEST_SRC_DIR=$(SMT_TEST_DIR)smt_utests/

BSQON_TEST_DIR=$(MAKE_PATH)/../test/
BSQON_TEST_SRC_DIR=$(BSQON_TEST_DIR)utests/

#dev is default, for another flavor : make BUILD=release or debug
BUILD := dev

CPP=g++ 
CPP_STDFLAGS=-Wall -Wextra -Wno-unused-parameter -Wuninitialized -Werror -std=gnu++20

CPPFLAGS_OPT.debug=-O0 -g -ggdb -fno-omit-frame-pointer -fsanitize=address
CPPFLAGS_OPT.dev=-O0 -g -ggdb -fno-omit-frame-pointer
CPPFLAGS_OPT.release=-O3 -march=x86-64-v3
CPPFLAGS=${CPPFLAGS_OPT.${BUILD}} ${CPP_STDFLAGS}
CPPFLAGS_TEST=${CPPFLAGS_OPT.dev} ${CPP_STDFLAGS}

CC=gcc 
CSTDFLAGS=-DEXPORT -std=gnu17

CFLAGS_OPT.debug=-O0 -g -ggdb -fno-omit-frame-pointer
CFLAGS_OPT.dev=-O0 -g -ggdb -fno-omit-frame-pointer
CFLAGS_OPT.release=-O3 -march=x86-64-v3
CFLAGS=${CFLAGS_OPT.${BUILD}} ${CSTDFLAGS}
CFLAGS_TEST=${CFLAGS_OPT.dev} ${CSTDFLAGS}

AR=ar
ARFLAGS=rs

CFG_HEADERS=$(PARSER_GEN_DIR)bytestring.h $(PARSER_GEN_DIR)bsqon_ast_common.h $(PARSER_GEN_DIR)bsqon_ast.h
CFG_SOURCES=$(PARSER_GEN_DIR)bsqon.y $(PARSER_GEN_DIR)bsqon.l

PARSER_HEADERS=$(PARSER_GEN_DIR)bytestring.h $(PARSER_GEN_DIR)bsqon_ast_common.h $(PARSER_GEN_DIR)bsqon_ast.h $(OUT_OBJ)bsqon.tab.h
PARSER_SOURCES=$(PARSER_GEN_DIR)bytestring.c $(PARSER_GEN_DIR)bsqon_ast_common.c $(PARSER_GEN_DIR)bsqon_ast.c $(OUT_OBJ)bsqon.tab.c $(OUT_OBJ)lex.yy.c
PARSER_OBJS=$(OUT_OBJ)bytestring.o $(OUT_OBJ)bsqon_ast_common.o $(OUT_OBJ)bsqon_ast.o $(OUT_OBJ)bsqon.tab.o $(OUT_OBJ)lex.yy.o

BSQON_HEADERS=$(SRC_DIR)common.h $(INFO_DIR)type_info.h $(INFO_DIR)bsqon.h $(PARSER_DIR)bsqon_parse.h
BSQON_SOURCES=$(INFO_DIR)type_info.cpp $(INFO_DIR)bsqon.cpp $(PARSER_DIR)bsqon_parse.cpp
BSQON_OBJS=$(OUT_OBJ)type_info.o $(OUT_OBJ)bsqon.o $(OUT_OBJ)bsqon_parse.o

GEN_HEADERS=$(GEN_DIR)component.h $(GEN_DIR)generator_rnd.h $(GEN_DIR)/generator_ai.h
GEN_SOURCES=$(GEN_DIR)component.cpp $(GEN_DIR)generator_rnd.cpp $(GEN_DIR)/generator_ai.cpp
GEN_OBJS=$(OUT_OBJ)component.o $(OUT_OBJ)generator_rnd.o $(OUT_OBJ)generator_ai.o

SMT_HEADERS=$(SMT_DIR)smt_utils.h $(SMT_DIR)smt_extract.h
SMT_SOURCES=$(SMT_DIR)smt_utils.cpp $(SMT_DIR)smt_extract.cpp
SMT_OBJS=$(OUT_OBJ)smt_utils.o	$(OUT_OBJ)smt_extract.o

SMT_TEST_SOURCES= $(BSQON_TEST_DIR)smt_test_driver.cpp $(SMT_TEST_SRC_DIR)extract_prim.cpp $(SMT_TEST_SRC_DIR)extract_dt.cpp $(SMT_TEST_SRC_DIR)extract_concept.cpp $(SMT_TEST_SRC_DIR)main.cpp $(SMT_TEST_SRC_DIR)generate_prim.cpp
BSQON_TEST_SOURCES=$(BSQON_TEST_DIR)test_driver.cpp $(BSQON_TEST_SRC_DIR)main.cpp $(BSQON_TEST_SRC_DIR)primitives.cpp $(BSQON_TEST_SRC_DIR)strings.cpp $(BSQON_TEST_SRC_DIR)typealias.cpp $(BSQON_TEST_SRC_DIR)specials.cpp $(BSQON_TEST_SRC_DIR)entity.cpp $(BSQON_TEST_SRC_DIR)datatype.cpp $(BSQON_TEST_SRC_DIR)collections.cpp $(BSQON_TEST_SRC_DIR)mixed.cpp

MAKEFLAGS += -j4

all: $(OUT_EXE)libbsqon.a $(OUT_EXE)bsqon $(OUT_EXE)tgen $(OUT_EXE)smtextract $(PCKG_DIR)libbsqon.a

$(PCKG_DIR)libbsqon.a: $(BSQON_HEADERS) $(PARSER_HEADERS) $(CFG_HEADERS) $(OUT_EXE)libbsqon.a
	@mkdir -p $(PCKG_DIR)
	@mkdir -p $(PCKG_DIR)fbparser/
	@mkdir -p $(PCKG_DIR)info/
	@mkdir -p $(PCKG_DIR)parser/
	cp $(OUT_EXE)libbsqon.a $(PCKG_DIR)libbsqon.a
	cp $(SRC_DIR)/common.h $(PCKG_DIR)/common.h
	cp $(CFG_HEADERS) $(PCKG_DIR)fbparser/
	cp $(BSQON_HEADERS) $(PCKG_DIR)info/
	cp $(PARSER_HEADERS) $(PCKG_DIR)parser/

$(OUT_EXE)tgen: $(GEN_HEADERS) $(GEN_OBJS) $(OUT_EXE)libbsqon.a $(GEN_DIR)tgen.cpp
	@mkdir -p $(OUT_EXE)
	$(CPP) $(CPPFLAGS) $(JSON_INCLUDES) $(BREX_INCLUDES) -L$(BREX_PATH) -L$(LIB_PATH) -o $(OUT_EXE)tgen $(GEN_OBJS) $(GEN_DIR)tgen.cpp -lbsqon -lbrex -lgmp -lcurl #-lfl, caused build err.

$(OUT_OBJ)component.o: $(GEN_HEADERS) $(GEN_SOURCES)
	@mkdir -p $(OUT_OBJ)
	$(CPP) $(CPPFLAGS) $(JSON_INCLUDES) $(BREX_INCLUDES) -o $(OUT_OBJ)component.o -c $(GEN_DIR)component.cpp

$(OUT_OBJ)generator_rnd.o: $(GEN_HEADERS) $(GEN_SOURCES)
	@mkdir -p $(OUT_OBJ)
	$(CPP) $(CPPFLAGS) $(JSON_INCLUDES) $(BREX_INCLUDES) -o $(OUT_OBJ)generator_rnd.o -c $(GEN_DIR)generator_rnd.cpp

$(OUT_OBJ)generator_ai.o: $(GEN_HEADERS) $(GEN_SOURCES)
	@mkdir -p $(OUT_OBJ)
	$(CPP) $(CPPFLAGS) $(JSON_INCLUDES) $(BREX_INCLUDES) -o $(OUT_OBJ)generator_ai.o -c $(GEN_DIR)generator_ai.cpp

$(OUT_EXE)bsqon: $(BSQON_HEADERS) $(PARSER_HEADERS) $(OUT_EXE)libbsqon.a $(PARSER_DIR)bsqon_main.cpp
	@mkdir -p $(OUT_EXE)
	$(CPP) $(CPPFLAGS) $(JSON_INCLUDES) $(BREX_INCLUDES) -L$(BREX_PATH) -L$(LIB_PATH) -o $(OUT_EXE)bsqon $(PARSER_DIR)bsqon_main.cpp -lbsqon -lbrex -lfl -lgmp

$(OUT_EXE)libbsqon.a: $(BSQON_OBJS) $(PARSER_OBJS)
	@mkdir -p $(OUT_EXE)
	$(AR) $(ARFLAGS) $(OUT_EXE)libbsqon.a $(BSQON_OBJS) $(PARSER_OBJS)

$(OUT_OBJ)bsqon_parse.o: $(BSQON_HEADERS) $(PARSER_HEADERS) $(PARSER_DIR)bsqon_parse.cpp
	@mkdir -p $(OUT_OBJ)
	$(CPP) $(CPPFLAGS) $(JSON_INCLUDES) $(BREX_INCLUDES) -o $(OUT_OBJ)bsqon_parse.o -c $(PARSER_DIR)bsqon_parse.cpp

$(OUT_OBJ)bsqon.o: $(BSQON_HEADERS) $(PARSER_HEADERS) $(INFO_DIR)bsqon.cpp
	@mkdir -p $(OUT_OBJ)
	$(CPP) $(CPPFLAGS) $(JSON_INCLUDES) $(BREX_INCLUDES) -o $(OUT_OBJ)bsqon.o -c $(INFO_DIR)bsqon.cpp

$(OUT_OBJ)type_info.o: $(BSQON_HEADERS) $(PARSER_HEADERS) $(INFO_DIR)type_info.cpp
	@mkdir -p $(OUT_OBJ)
	$(CPP) $(CPPFLAGS) $(JSON_INCLUDES) $(BREX_INCLUDES) -o $(OUT_OBJ)type_info.o -c $(INFO_DIR)type_info.cpp

$(OUT_OBJ)bytestring.o: $(PARSER_HEADERS) $(PARSER_GEN_DIR)bytestring.c
	@mkdir -p $(OUT_OBJ)
	$(CC) $(CFLAGS) -o $(OUT_OBJ)bytestring.o -c $(PARSER_GEN_DIR)bytestring.c

$(OUT_OBJ)bsqon_ast_common.o: $(PARSER_HEADERS) $(PARSER_GEN_DIR)bsqon_ast_common.c
	@mkdir -p $(OUT_OBJ)
	$(CC) $(CFLAGS) -o $(OUT_OBJ)bsqon_ast_common.o -c $(PARSER_GEN_DIR)bsqon_ast_common.c

$(OUT_OBJ)bsqon_ast.o: $(PARSER_HEADERS) $(PARSER_GEN_DIR)bsqon_ast.c
	@mkdir -p $(OUT_OBJ)
	$(CC) $(CFLAGS) -o $(OUT_OBJ)bsqon_ast.o -c $(PARSER_GEN_DIR)bsqon_ast.c

$(OUT_OBJ)bsqon.tab.o: $(PARSER_HEADERS) $(OUT_OBJ)bsqon.tab.c
	@mkdir -p $(OUT_OBJ)
	$(CC) $(CFLAGS) -o $(OUT_OBJ)bsqon.tab.o -c $(OUT_OBJ)bsqon.tab.c

$(OUT_OBJ)lex.yy.o: $(PARSER_HEADERS) $(OUT_OBJ)lex.yy.c
	@mkdir -p $(OUT_OBJ)
	$(CC) $(CFLAGS) -o $(OUT_OBJ)lex.yy.o -c $(OUT_OBJ)lex.yy.c

$(OUT_OBJ)bsqon.tab.h $(OUT_OBJ)bsqon.tab.c &: $(CFG_HEADERS) $(CFG_SOURCES)
	@mkdir -p $(OUT_OBJ)
	bison -d -Wcex -o $(OUT_OBJ)bsqon.tab.c $(PARSER_GEN_DIR)bsqon.y

$(OUT_OBJ)lex.yy.c: $(CFG_HEADERS) $(CFG_SOURCES)
	@mkdir -p $(OUT_OBJ)
	flex --warn -o $(OUT_OBJ)lex.yy.c $(PARSER_GEN_DIR)bsqon.l

$(OUT_EXE)smtextract: $(SMT_HEADERS) $(OUT_EXE)libbsqon.a $(SMT_DIR)smt_main.cpp $(SMT_OBJS)
	@mkdir -p $(OUT_EXE)
	$(CPP) $(CPPFLAGS) $(JSON_INCLUDES) $(BREX_INCLUDES) $(Z3_INCLUDES) -L$(Z3_PATH) -L$(BREX_PATH) -L$(LIB_PATH) -o $(OUT_EXE)smtextract $(SMT_OBJS) $(SMT_DIR)smt_main.cpp -lbsqon -lbrex -lgmp -lcurl -lz3 #-lfl, caused build err.

$(OUT_OBJ)smt_utils.o: $(SMT_HEADERS) $(SMT_SOURCES)
	@mkdir -p $(OUT_OBJ)
	$(CPP) $(CPPFLAGS) $(JSON_INCLUDES) $(BREX_INCLUDES) $(Z3_INCLUDES) -o $(OUT_OBJ)smt_utils.o -c $(SMT_DIR)smt_utils.cpp

$(OUT_OBJ)smt_extract.o: $(SMT_HEADERS) $(SMT_SOURCES)
	@mkdir -p $(OUT_OBJ)
	$(CPP) $(CPPFLAGS) $(JSON_INCLUDES) $(BREX_INCLUDES) $(Z3_INCLUDES) -o $(OUT_OBJ)smt_extract.o -c $(SMT_DIR)smt_extract.cpp

smt_test_files: $(SMT_HEADERS) $(PARSER_HEADERS) $(OUT_EXE)libbsqon.a $(BSQON_TEST_DIR)smt_test_driver.h $(SMT_TEST_SOURCES) $(SMT_SOURCES)
	@rm -rf $(BIN_DIR)
	@mkdir -p $(BIN_DIR)
	cp -r $(SMT_TEST_DIR)asm_meta $(BIN_DIR)asm_meta
	cp -r $(SMT_TEST_DIR)asm_meta/smt/ $(BIN_DIR)asm_meta/smt/
	cp -r $(SMT_TEST_DIR)smt_utests/ $(BIN_DIR)smt_utests/
	$(CPP) $(CPPFLAGS_TEST) -DTEST_PATH='"$(BIN_DIR)"' $(JSON_INCLUDES) $(BREX_INCLUDES) -L$(BREX_PATH) -L$(LIB_PATH) -o $(BIN_DIR)smtextract_test $(SMT_TEST_SOURCES) $(SMT_SOURCES) $(OUT_EXE)libbsqon.a -lbsqon -lbrex -lgmp -lz3 -lboost_unit_test_framework #-lfl

smt_test: smt_test_files
	$(BIN_DIR)smtextract_test --report_level=short --color_output

testfiles: $(BSQON_HEADERS) $(PARSER_HEADERS) $(OUT_EXE)libbsqon.a $(BSQON_TEST_DIR)test_driver.h $(BSQON_TEST_SOURCES)
	@rm -rf $(BIN_DIR)
	@mkdir -p $(BIN_DIR)
	cp -r $(BSQON_TEST_DIR)asm_meta/ $(BIN_DIR)asm_meta/
	cp -r $(BSQON_TEST_DIR)bsqon/ $(BIN_DIR)bsqon/
	$(CPP) $(CPPFLAGS_TEST) -DTEST_PATH='"$(BIN_DIR)"' $(JSON_INCLUDES) $(BREX_INCLUDES) -L$(BREX_PATH) -L$(LIB_PATH) -o $(BIN_DIR)bsqon_test $(BSQON_TEST_SOURCES) $(OUT_EXE)libbsqon.a -lbsqon -lbrex -lfl -lgmp -lboost_unit_test_framework

test: testfiles
	$(BIN_DIR)bsqon_test --report_level=short --color_output

clean:
	rm -rf $(OUT_EXE)* $(OUT_OBJ)*.o $(BIN_DIR)*
