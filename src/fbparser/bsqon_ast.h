#pragma once

#include "bsqon_ast_common.h"
#include "bytestring.h"

#ifdef __cplusplus
extern "C" 
{
#endif

enum BSQON_AST_TAG
{
    BSQON_AST_TAG_Error = 1,

    BSQON_AST_TAG_NominalType,
    BSQON_AST_TAG_NominalScopedType,
    BSQON_AST_TAG_EListType,

    BSQON_AST_TAG_NoneValue,
    BSQON_AST_TAG_TrueValue,
    BSQON_AST_TAG_FalseValue,
    BSQON_AST_TAG_NatValue,
    BSQON_AST_TAG_IntValue,
    BSQON_AST_TAG_BigNatValue,
    BSQON_AST_TAG_BigIntValue,
    BSQON_AST_TAG_RationalValue,
    BSQON_AST_TAG_FloatValue,
    BSQON_AST_TAG_DecimalValue,
    BSQON_AST_TAG_DecimalDegreeValue,
    BSQON_AST_TAG_LatLongValue,
    BSQON_AST_TAG_ComplexValue,
    BSQON_AST_TAG_NumberinoValue,
    BSQON_AST_TAG_ByteBufferValue,
    BSQON_AST_TAG_UUIDv4Value,
    BSQON_AST_TAG_UUIDv7Value,
    BSQON_AST_TAG_SHAHashcodeValue,
    BSQON_AST_TAG_StringValue,
    BSQON_AST_TAG_CStringValue,
    BSQON_AST_TAG_PathValue,
    BSQON_AST_TAG_RegexValue,
    BSQON_AST_TAG_TZDateTimeValue,
    BSQON_AST_TAG_TIATimeValue,
    BSQON_AST_TAG_PlainDateValue,
    BSQON_AST_TAG_PlainTimeValue,
    BSQON_AST_TAG_LogicalTimeValue,
    BSQON_AST_TAG_TimestampValue,

    BSQON_AST_TAG_DeltaDateTimeValue,
    BSQON_AST_TAG_DeltaSecondsValue,
    BSQON_AST_TAG_DeltaLogicalValue,
    BSQON_AST_TAG_DeltaISOTimeStampValue,

    BSQON_AST_TAG_IdentifierValue,
    BSQON_AST_TAG_UnspecIdentifierValue,
    
    BSQON_AST_TAG_TypedLiteralValue,

    BSQON_AST_TAG_MapEntryValue,
    BSQON_AST_TAG_BracketValue,
    BSQON_AST_TAG_BraceValue,
    BSQON_AST_TAG_TypedValue,

    BSQON_AST_TAG_SomeConsValue,
    BSQON_AST_TAG_OkConsValue,
    BSQON_AST_TAG_ErrConsValue,

    BSQON_AST_TAG_EnvAccessValue,

    BSQON_AST_TAG_LetInValue,
    BSQON_AST_TAG_AccessNameValue,
    BSQON_AST_TAG_AccessIndexValue,
    BSQON_AST_TAG_AccessKeyValue,

    BSQON_AST_TAG_ScopedNameValue,
    BSQON_AST_TAG_EnumAccessValue,

    BSQON_AST_TAG_BsqonDeclBody
};

struct BSQON_AST_Node
{
    enum BSQON_AST_TAG tag;
    struct AST_SourcePos pos;
};

BSQON_AST_NODE_DECLARE_0(ErrorNode)
#define BSQON_AST_ERROR(SL) BSQON_AST_NODE_CONS(ErrorNode, BSQON_AST_TAG_Error, SL)

BSQON_AST_LIST_DECLARE(BSQON_AST_Node)
BSQON_AST_NLIST_DECLARE(BSQON_AST_Node)

////////
//Type nodes
#define BSQON_AST_LIST_OF_TYPES BSQON_LIST(BSQON_AST_Node)
#define BSQON_AST_LIST_OF_TYPES_Empty BSQON_LIST_Empty(BSQON_AST_Node)
#define BSQON_AST_LIST_OF_TYPES_Singleton(V) BSQON_LIST_Singleton(BSQON_AST_Node, V)
#define BSQON_AST_LIST_OF_TYPES_Push(V, L) BSQON_LIST_Push(BSQON_AST_Node, V, L)
#define BSQON_AST_LIST_OF_TYPES_Reverse(L) BSQON_LIST_Reverse(BSQON_AST_Node, L)
#define BSQON_AST_LIST_OF_TYPES_Print(L) BSQON_LIST_Print(BSQON_AST_Node, L)

#define BSQON_AST_NLIST_OF_TYPES_ENTRY BSQON_NLIST_ENTRY(BSQON_AST_Node)
#define BSQON_AST_NLIST_OF_TYPES_ENTRY_Create(N, V) BSQON_NLIST_ENTRY_Create(BSQON_AST_Node, N, V)

#define BSQON_AST_NLIST_OF_TYPES BSQON_NLIST(BSQON_AST_Node)
#define BSQON_AST_NLIST_OF_TYPES_Empty BSQON_NLIST_Empty(BSQON_AST_Node)
#define BSQON_AST_NLIST_OF_TYPES_Singleton(V) BSQON_NLIST_Singleton(BSQON_AST_Node, V)
#define BSQON_AST_NLIST_OF_TYPES_Push(V, L) BSQON_NLIST_Push(BSQON_AST_Node, V, L)
#define BSQON_AST_NLIST_OF_TYPES_Reverse(L) BSQON_NLIST_Reverse(BSQON_AST_Node, L)
#define BSQON_AST_NLIST_OF_TYPES_Print(L) BSQON_NLIST_Print(BSQON_AST_Node, L)

BSQON_AST_NODE_DECLARE_2(NominalType, const char*, name, struct BSQON_AST_LIST_OF_TYPES*, terms)
BSQON_AST_NODE_DECLARE_2(NominalScopedType, struct BSQON_AST_Node*, root, const char*, ext)
BSQON_AST_NODE_DECLARE_1(EListType, struct BSQON_AST_LIST_OF_TYPES*, types)

//Value Nodes
#define BSQON_AST_LIST_OF_VALUES BSQON_LIST(BSQON_AST_Node)
#define BSQON_AST_LIST_OF_VALUES_Empty BSQON_LIST_Empty(BSQON_AST_Node)
#define BSQON_AST_LIST_OF_VALUES_Singleton(V) BSQON_LIST_Singleton(BSQON_AST_Node, V)
#define BSQON_AST_LIST_OF_VALUES_Push(V, L) BSQON_LIST_Push(BSQON_AST_Node, V, L)
#define BSQON_AST_LIST_OF_VALUES_Reverse(L) BSQON_LIST_Reverse(BSQON_AST_Node, L)
#define BSQON_AST_LIST_OF_VALUES_Print(L) BSQON_LIST_Print(BSQON_AST_Node, L)

#define BSQON_AST_NLIST_OF_VALUES_ENTRY BSQON_NLIST_ENTRY(BSQON_AST_Node)
#define BSQON_AST_NLIST_OF_VALUES_ENTRY_Create(N, V) BSQON_NLIST_ENTRY_Create(BSQON_AST_Node, N, V)

#define BSQON_AST_NLIST_OF_VALUES BSQON_NLIST(BSQON_AST_Node)
#define BSQON_AST_NLIST_OF_VALUES_Empty BSQON_NLIST_Empty(BSQON_AST_Node)
#define BSQON_AST_NLIST_OF_VALUES_Singleton(V) BSQON_NLIST_Singleton(BSQON_AST_Node, V)
#define BSQON_AST_NLIST_OF_VALUES_Push(V, L) BSQON_NLIST_Push(BSQON_AST_Node, V, L)
#define BSQON_AST_NLIST_OF_VALUES_Reverse(L) BSQON_NLIST_Reverse(BSQON_AST_Node, L)
#define BSQON_AST_NLIST_OF_VALUES_Print(L) BSQON_NLIST_Print(BSQON_AST_Node, L)

BSQON_AST_NODE_DECLARE_0(SingletonValue)
BSQON_AST_NODE_DECLARE_1(LiteralStandardValue, const char*, data)
BSQON_AST_NODE_DECLARE_1(LiteralStringValue, struct ByteString*, data)
BSQON_AST_NODE_DECLARE_1(LiteralPathValue, struct ByteString*, data)

BSQON_AST_NODE_DECLARE_1(NameValue, const char*, data)
BSQON_AST_NODE_DECLARE_2(TypedLiteralValue, struct BSQON_AST_Node*, data, struct BSQON_AST_Node*, type)
BSQON_AST_NODE_DECLARE_2(MapEntryValue, struct BSQON_AST_Node*, key, struct BSQON_AST_Node*, value)
BSQON_AST_NODE_DECLARE_1(BracketValue, struct BSQON_AST_LIST_OF_VALUES*, values)
BSQON_AST_NODE_DECLARE_1(BraceValue, struct BSQON_AST_NLIST_OF_VALUES*, entries)
BSQON_AST_NODE_DECLARE_3(TypedValue, struct BSQON_AST_Node*, value, struct BSQON_AST_Node*, type, bool, istagged)
BSQON_AST_NODE_DECLARE_2(SpecialConsValue, struct BSQON_AST_Node*, value, const char*, consname)
BSQON_AST_NODE_DECLARE_2(EnvAccessValue, struct ByteString*, data, struct BSQON_AST_Node*, type)

BSQON_AST_NODE_DECLARE_2(ScopedNameValue, struct BSQON_AST_Node*, root, const char*, identifier)
BSQON_AST_NODE_DECLARE_2(EnumAccessValue, struct BSQON_AST_Node*, root, const char*, identifier)
BSQON_AST_NODE_DECLARE_4(LetInValue, const char*, vname, struct BSQON_AST_Node*, vtype, struct BSQON_AST_Node*, value, struct BSQON_AST_Node*, exp)

BSQON_AST_NODE_DECLARE_2(AccessNameValue, struct BSQON_AST_Node*, value, const char*, name)
BSQON_AST_NODE_DECLARE_2(AccessIndexValue, struct BSQON_AST_Node*, value, const char*, idx)
BSQON_AST_NODE_DECLARE_2(AccessKeyValue, struct BSQON_AST_Node*, value, struct BSQON_AST_Node*, kk)

//DeclNodes
BSQON_AST_NODE_DECLARE_3(BsqonDeclBody, const char*, shebangmeta, struct BSQON_AST_NLIST_OF_TYPES*, envtypes, struct BSQON_AST_Node*, value)

enum BSQON_AST_TAG BSQON_AST_getTag(const struct BSQON_AST_Node* node);
const char* BSQON_AST_getTagName(const struct BSQON_AST_Node* node);

void BSQON_AST_print(const struct BSQON_AST_Node* node);

const struct BSQON_AST_Node* BSQON_AST_parse_from_stdin();
const struct BSQON_AST_Node* BSQON_AST_parse_from_file(const char* file);

size_t BSQON_AST_getErrorInfo(char** errorInfo);

#ifdef __cplusplus
}
#endif
