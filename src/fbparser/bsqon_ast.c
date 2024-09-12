#include "bsqon_ast.h"

#include <stdio.h>
#include <assert.h>

BSQON_AST_NODE_DEFINE_0(ErrorNode)

BSQON_AST_LIST_DEFINE(BSQON_AST_Node)
BSQON_AST_NLIST_DEFINE(BSQON_AST_Node)

BSQON_AST_NODE_DEFINE_2(NominalType, const char*, name, struct BSQON_AST_LIST_OF_TYPES*, terms)
BSQON_AST_NODE_DEFINE_2(NominalScopedType, struct BSQON_AST_Node*, root, const char*, ext)
BSQON_AST_NODE_DEFINE_1(EListType, struct BSQON_AST_LIST_OF_TYPES*, types)

BSQON_AST_NODE_DEFINE_0(SingletonValue)
BSQON_AST_NODE_DEFINE_1(LiteralStandardValue, const char*, data)
BSQON_AST_NODE_DEFINE_1(LiteralStringValue, struct ByteString*, data)
BSQON_AST_NODE_DEFINE_1(LiteralPathValue, struct ByteString*, data)

BSQON_AST_NODE_DEFINE_1(NameValue, const char*, data)
BSQON_AST_NODE_DEFINE_2(TypedLiteralValue, struct BSQON_AST_Node*, data, struct BSQON_AST_Node*, type)
BSQON_AST_NODE_DEFINE_2(MapEntryValue, struct BSQON_AST_Node*, key, struct BSQON_AST_Node*, value)
BSQON_AST_NODE_DEFINE_1(BracketValue, struct BSQON_AST_LIST_OF_VALUES*, values)
BSQON_AST_NODE_DEFINE_1(BraceValue, struct BSQON_AST_NLIST_OF_VALUES*, entries)
BSQON_AST_NODE_DEFINE_3(TypedValue, struct BSQON_AST_Node*, value, struct BSQON_AST_Node*, type, bool, istagged)
BSQON_AST_NODE_DEFINE_2(SpecialConsValue, struct BSQON_AST_Node*, value, const char*, consname)
BSQON_AST_NODE_DEFINE_2(EnvAccessValue, struct ByteString*, data, struct BSQON_AST_Node*, type)

BSQON_AST_NODE_DEFINE_2(ScopedNameValue, struct BSQON_AST_Node*, root, const char*, identifier)
BSQON_AST_NODE_DEFINE_4(LetInValue, const char*, vname, struct BSQON_AST_Node*, vtype, struct BSQON_AST_Node*, value, struct BSQON_AST_Node*, exp)

BSQON_AST_NODE_DEFINE_2(AccessNameValue, struct BSQON_AST_Node*, value, const char*, name)
BSQON_AST_NODE_DEFINE_2(AccessIndexValue, struct BSQON_AST_Node*, value, const char*, idx)
BSQON_AST_NODE_DEFINE_2(AccessKeyValue, struct BSQON_AST_Node*, value, struct BSQON_AST_Node*, kk)

BSQON_AST_NODE_DEFINE_3(BsqonDeclBody, const char*, shebangmeta, struct BSQON_AST_NLIST_OF_TYPES*, envtypes, struct BSQON_AST_Node*, value)

enum BSQON_AST_TAG BSQON_AST_getTag(const struct BSQON_AST_Node* node)
{
    return node->tag;
}

const char* BSQON_AST_getTagName(const struct BSQON_AST_Node* node)
{
    switch (node->tag)
    {
        case BSQON_AST_TAG_Error: return "^ERROR_NODE^";
        case BSQON_AST_TAG_NominalType: return "NominalType";
        case BSQON_AST_TAG_NominalScopedType: return "NominalScopedType";
        case BSQON_AST_TAG_EListType: return "EListType";
        
        case BSQON_AST_TAG_NoneValue: return "NoneValue";
        case BSQON_AST_TAG_TrueValue: return "TrueValue";
        case BSQON_AST_TAG_FalseValue: return "FalseValue";
        case BSQON_AST_TAG_NatValue: return "NatValue";
        case BSQON_AST_TAG_IntValue: return "IntValue";
        case BSQON_AST_TAG_BigNatValue: return "BigNatValue";
        case BSQON_AST_TAG_BigIntValue: return "BigIntValue";
        case BSQON_AST_TAG_RationalValue: return "RationalValue";
        case BSQON_AST_TAG_FloatValue: return "FloatValue";
        case BSQON_AST_TAG_DecimalValue: return "DecimalValue";
        case BSQON_AST_TAG_DecimalDegreeValue: return "DecimalDegreeValue";
        case BSQON_AST_TAG_LatLongValue: return "LatLongValue";
        case BSQON_AST_TAG_ComplexValue: return "ComplexValue";
        case BSQON_AST_TAG_NumberinoValue: return "NumberinoValue";
        case BSQON_AST_TAG_ByteBufferValue: return "ByteBufferValue";
        case BSQON_AST_TAG_UUIDv4Value: return "UUIDv4Value";
        case BSQON_AST_TAG_UUIDv7Value: return "UUIDv7Value";
        case BSQON_AST_TAG_SHAHashcodeValue: return "SHAHashcodeValue";
        case BSQON_AST_TAG_StringValue: return "StringValue";
        case BSQON_AST_TAG_CStringValue: return "CStringValue";
        case BSQON_AST_TAG_RegexValue: return "RegexValue";
        case BSQON_AST_TAG_TZDateTimeValue: return "TZDateTimeValue";
        case BSQON_AST_TAG_TIATimeValue: return "TIATimeValue";
        case BSQON_AST_TAG_PlainDateValue: return "PlainDateValue";
        case BSQON_AST_TAG_PlainTimeValue: return "PlainTimeValue";
        case BSQON_AST_TAG_LogicalTimeValue: return "LogicalTimeValue";
        case BSQON_AST_TAG_TimestampValue: return "TimestampValue";
        case BSQON_AST_TAG_DeltaDateTimeValue: return "DeltaDateTimeValue";
        case BSQON_AST_TAG_DeltaSecondsValue: return "DeltaSecondsValue";
        case BSQON_AST_TAG_DeltaLogicalValue: return "DeltaLogicalValue";
        case BSQON_AST_TAG_DeltaISOTimeStampValue: return "DeltaISOTimeStampValue";
        case BSQON_AST_TAG_LiteralPathValue: return "LiteralPathValue";
        case BSQON_AST_TAG_IdentifierValue: return "IdentifierValue";
        case BSQON_AST_TAG_UnspecIdentifierValue: return "UnspecIdentifierValue";
        case BSQON_AST_TAG_TypedLiteralValue: return "TypedLiteralValue";
        case BSQON_AST_TAG_MapEntryValue: return "MapEntryValue";
        case BSQON_AST_TAG_BracketValue: return "BracketValue";
        case BSQON_AST_TAG_BraceValue: return "BraceValue";
        case BSQON_AST_TAG_TypedValue: return "TypedValue";
        case BSQON_AST_TAG_SomeConsValue: return "SomeConsValue";
        case BSQON_AST_TAG_OkConsValue: return "OkConsValue";
        case BSQON_AST_TAG_ErrConsValue: return "ErrConsValue";
        case BSQON_AST_TAG_EnvAccessValue: return "EnvAccessValue";
        case BSQON_AST_TAG_LetInValue: return "LetInValue";
        case BSQON_AST_TAG_AccessNameValue: return "AccessNameValue";
        case BSQON_AST_TAG_AccessIndexValue: return "AccessIndexValue";
        case BSQON_AST_TAG_AccessKeyValue: return "AccessKeyValue";
        case BSQON_AST_TAG_ScopedNameValue: return "ScopedNameValue";
        default: return "^^MISSING_CASE^^";
    }
}

void BSQON_AST_print(const struct BSQON_AST_Node* node)
{
    switch (node->tag)
    {
    case BSQON_AST_TAG_Error: {
        printf("^ERROR_EXP^");
        break;
    }
    case BSQON_AST_TAG_NominalType: {
        const struct BSQON_AST_NODE(NominalType)* nn = BSQON_AST_NODE_AS(NominalType, node);
        printf("%s", nn->name);
        if(nn->terms != NULL) {
            printf("<");
            BSQON_AST_LIST_OF_TYPES_Print(nn->terms);
            printf(">");
        }
        break;
    }
    case BSQON_AST_TAG_NominalScopedType: {
        const struct BSQON_AST_NODE(NominalScopedType)* nn = BSQON_AST_NODE_AS(NominalScopedType, node);

        BSQON_AST_print(nn->root);
        printf("::%s", nn->ext);
        break;
    }
    case BSQON_AST_TAG_EListType: {
        const struct BSQON_AST_NODE(EListType)* nn = BSQON_AST_NODE_AS(EListType, node);

        printf("(|");
        BSQON_AST_LIST_OF_TYPES_Print(nn->types);
        printf("|)");
        break;
    }
    case BSQON_AST_TAG_NoneValue: 
    case BSQON_AST_TAG_TrueValue:
    case BSQON_AST_TAG_FalseValue: {
        printf("%s", BSQON_AST_getTagName(node));
        break;
    }
    case BSQON_AST_TAG_NatValue:
    case BSQON_AST_TAG_IntValue:
    case BSQON_AST_TAG_BigNatValue:
    case BSQON_AST_TAG_BigIntValue:
    case BSQON_AST_TAG_RationalValue:
    case BSQON_AST_TAG_FloatValue:
    case BSQON_AST_TAG_DecimalValue:
    case BSQON_AST_TAG_DecimalDegreeValue:
    case BSQON_AST_TAG_LatLongValue:
    case BSQON_AST_TAG_ComplexValue:
    case BSQON_AST_TAG_NumberinoValue:
    case BSQON_AST_TAG_ByteBufferValue:
    case BSQON_AST_TAG_UUIDv4Value:
    case BSQON_AST_TAG_UUIDv7Value:
    case BSQON_AST_TAG_SHAHashcodeValue: {
        const struct BSQON_AST_NODE(LiteralStandardValue)* nn = BSQON_AST_NODE_AS(LiteralStandardValue, node);
        printf("%s", nn->data);
        break;
    }
    case BSQON_AST_TAG_StringValue:
    case BSQON_AST_TAG_CStringValue:
    case BSQON_AST_TAG_RegexValue: {
        const struct BSQON_AST_NODE(LiteralStringValue)* nn = BSQON_AST_NODE_AS(LiteralStringValue, node);
        printf("%s", nn->data->bytes);
        break;
    }
    case BSQON_AST_TAG_TZDateTimeValue:
    case BSQON_AST_TAG_TIATimeValue:
    case BSQON_AST_TAG_PlainDateValue:
    case BSQON_AST_TAG_PlainTimeValue:
    case BSQON_AST_TAG_LogicalTimeValue:
    case BSQON_AST_TAG_TimestampValue: 
    case BSQON_AST_TAG_DeltaDateTimeValue:
    case BSQON_AST_TAG_DeltaSecondsValue:
    case BSQON_AST_TAG_DeltaLogicalValue:
    case BSQON_AST_TAG_DeltaISOTimeStampValue: {
        const struct BSQON_AST_NODE(LiteralStandardValue)* nn = BSQON_AST_NODE_AS(LiteralStandardValue, node);
        printf("%s", nn->data);
        break;
    }
    case BSQON_AST_TAG_LiteralPathValue: {
        const struct BSQON_AST_NODE(LiteralPathValue)* nn = BSQON_AST_NODE_AS(LiteralPathValue, node);
        printf("%s", nn->data->bytes);
        break;
    }
    case BSQON_AST_TAG_IdentifierValue:
    case BSQON_AST_TAG_UnspecIdentifierValue: {
        const struct BSQON_AST_NODE(NameValue)* nn = BSQON_AST_NODE_AS(NameValue, node);
        printf("%s", nn->data);
        break;
    }
    case BSQON_AST_TAG_TypedLiteralValue: {
        const struct BSQON_AST_NODE(TypedLiteralValue)* nn = BSQON_AST_NODE_AS(TypedLiteralValue, node);
        BSQON_AST_print(nn->data);
        printf("<");
        BSQON_AST_print(nn->type);
        printf(">");
        break;
    }
    case BSQON_AST_TAG_MapEntryValue: {
        const struct BSQON_AST_NODE(MapEntryValue)* nn = BSQON_AST_NODE_AS(MapEntryValue, node);
        BSQON_AST_print(nn->key);
        printf(" => ");
        BSQON_AST_print(nn->value);
        break;
    }
    case BSQON_AST_TAG_BracketValue: {
        const struct BSQON_AST_NODE(BracketValue)* nn = BSQON_AST_NODE_AS(BracketValue, node);
        printf("[");
        BSQON_AST_LIST_OF_TYPES_Print(nn->values);
        printf("]");
        break;
    }
    case BSQON_AST_TAG_BraceValue: {
        const struct BSQON_AST_NODE(BraceValue)* nn = BSQON_AST_NODE_AS(BraceValue, node);
        printf("{");
        BSQON_AST_NLIST_OF_VALUES_Print(nn->entries);
        printf("}");
        break;
    }
    case BSQON_AST_TAG_TypedValue: {
        const struct BSQON_AST_NODE(TypedValue)* nn = BSQON_AST_NODE_AS(TypedValue, node);
        if(nn->type->tag != BSQON_AST_TAG_NominalType && nn->type->tag != BSQON_AST_TAG_NominalScopedType) {
            printf("<");
        }

        BSQON_AST_print(nn->type);
    
        if(nn->type->tag != BSQON_AST_TAG_NominalType && nn->type->tag != BSQON_AST_TAG_NominalScopedType) {
            printf(">");
        }

        BSQON_AST_print(nn->value);
        break;
    }
    case BSQON_AST_TAG_SomeConsValue:
    case BSQON_AST_TAG_OkConsValue:
    case BSQON_AST_TAG_ErrConsValue: {
        const struct BSQON_AST_NODE(SpecialConsValue)* nn = BSQON_AST_NODE_AS(SpecialConsValue, node);
        printf("%s(", nn->consname);
        BSQON_AST_print(nn->value);
        printf(")");
        break;
    }
    case BSQON_AST_TAG_EnvAccessValue: {
        const struct BSQON_AST_NODE(EnvAccessValue)* nn = BSQON_AST_NODE_AS(EnvAccessValue, node);
        printf("env");
        if(nn->type != NULL) {
            printf("<");
            BSQON_AST_print(nn->type);
            printf(">");
        }

        printf("[|%s|]", nn->data->bytes);
        break;
    }
    case BSQON_AST_TAG_LetInValue: {
        const struct BSQON_AST_NODE(LetInValue)* nn = BSQON_AST_NODE_AS(LetInValue, node);
        printf("let %s: ", nn->vname);
        BSQON_AST_print(nn->vtype);
        printf(" = ");
        BSQON_AST_print(nn->value);
        printf(" in ");
        BSQON_AST_print(nn->exp);
        break;
    }
    case BSQON_AST_TAG_AccessNameValue: {
        const struct BSQON_AST_NODE(AccessNameValue)* nn = BSQON_AST_NODE_AS(AccessNameValue, node);
        BSQON_AST_print(nn->value);
        printf(".%s", nn->name);
        break;
    }
    case BSQON_AST_TAG_AccessIndexValue: {
        const struct BSQON_AST_NODE(AccessIndexValue)* nn = BSQON_AST_NODE_AS(AccessIndexValue, node);
        BSQON_AST_print(nn->value);
        printf("[|%s|]", nn->idx);
        break;
    }
    case BSQON_AST_TAG_AccessKeyValue: {
        const struct BSQON_AST_NODE(AccessKeyValue)* nn = BSQON_AST_NODE_AS(AccessKeyValue, node);
        BSQON_AST_print(nn->value);
        printf("[|");
        BSQON_AST_print(nn->kk);
        printf("|]");
        break;
    }
    case BSQON_AST_TAG_ScopedNameValue: {
        const struct BSQON_AST_NODE(ScopedNameValue)* nn = BSQON_AST_NODE_AS(ScopedNameValue, node);
        BSQON_AST_print(nn->root);
        printf("::%s", nn->identifier);
        break;
    }
    case BSQON_AST_TAG_BsqonDeclBody: {
        const struct BSQON_AST_NODE(BsqonDeclBody)* nn = BSQON_AST_NODE_AS(BsqonDeclBody, node);
        if(nn->shebangmeta != NULL) {
            printf("#!%s", nn->shebangmeta);
        }

        if(nn->envtypes != NULL) {
            if(nn->shebangmeta != NULL)
            {
                printf("\n");
            }

            printf("env{");
            BSQON_AST_NLIST_OF_TYPES_Print(nn->envtypes);
            printf("}");
        }

        if(nn->shebangmeta != NULL || nn->envtypes != NULL)
        {
            printf("\n");
        }
        BSQON_AST_print(nn->value);
        break;
    }
    default:
        printf("^^MISSING_CASE PRINT_AST_NODE^^");
        break;
    }
}
