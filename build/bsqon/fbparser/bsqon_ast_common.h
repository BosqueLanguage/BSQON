#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#pragma once

////////////////////////////
//ALLOCATION Macros

#define AST_ALLOC_ALIGN_8(size) (((size) + 7) & ~7)
#define AST_ALLOC(size) malloc(AST_ALLOC_ALIGN_8(size))

#define AST_STRDUP(str) strdup(str)

struct AST_SourcePos
{
    uint32_t first_line;
    uint32_t first_column;
    uint32_t last_line;
    uint32_t last_column;
};

struct AST_SourcePos createSourcePos(uint32_t first_line, uint32_t first_column, uint32_t last_line, uint32_t last_column);

////////////////////////////
//List Macros

#define BSQON_LIST(T) BSQON_LIST_OF_##T

#define BSQON_NLIST_ENTRY(T) BSQON_NLIST_ENTRY_OF_##T
#define BSQON_NLIST(T) BSQON_NLIST_OF_##T

#define BSQON_AST_LIST_DECLARE(T) \
struct BSQON_LIST(T) \
{ \
    struct T* value; \
    struct BSQON_LIST(T)* next; \
}; \
struct BSQON_LIST(T)* BSQON_LIST_##T##Singleton(struct T* value); \
struct BSQON_LIST(T)* BSQON_LIST_##T##Push(struct T* value, struct BSQON_LIST(T)* ll); \
struct BSQON_LIST(T)* BSQON_LIST_##T##Reverse(struct BSQON_LIST(T)* ll); \
void BSQON_LIST_##T##Print(struct BSQON_LIST(T)* ll);

#define BSQON_LIST_Empty(T) NULL
#define BSQON_LIST_Singleton(T, V) BSQON_LIST_##T##Singleton(V)
#define BSQON_LIST_Push(T, V, L) BSQON_LIST_##T##Push(V, L)
#define BSQON_LIST_Reverse(T, L) BSQON_LIST_##T##Reverse(L)
#define BSQON_LIST_Print(T, L) BSQON_LIST_##T##Print(L)

#define BSQON_AST_NLIST_DECLARE(T) \
struct BSQON_NLIST_ENTRY(T) \
{ \
    const char* name; \
    struct T* value; \
}; \
struct BSQON_NLIST_ENTRY(T) BSQON_NLIST_##T##Create(const char* name, struct T* value); \
struct BSQON_NLIST(T) \
{ \
    struct BSQON_NLIST_ENTRY(T) entry; \
    struct BSQON_NLIST(T)* next; \
}; \
struct BSQON_NLIST(T)* BSQON_NLIST_##T##Singleton(struct BSQON_NLIST_ENTRY(T) entry); \
struct BSQON_NLIST(T)* BSQON_NLIST_##T##Push(struct BSQON_NLIST_ENTRY(T) entry, struct BSQON_NLIST(T)* ll); \
struct BSQON_NLIST(T)* BSQON_NLIST_##T##Reverse(struct BSQON_NLIST(T)* ll); \
void BSQON_NLIST_##T##Print(struct BSQON_NLIST(T)* ll);

#define BSQON_NLIST_ENTRY_Create(T, N, V) BSQON_NLIST_##T##Create(N, V)

#define BSQON_NLIST_Empty(T) NULL
#define BSQON_NLIST_Singleton(T, E) BSQON_NLIST_##T##Singleton(E)
#define BSQON_NLIST_Push(T, E, L) BSQON_NLIST_##T##Push(E, L)
#define BSQON_NLIST_Reverse(T, L) BSQON_NLIST_##T##Reverse(L)
#define BSQON_NLIST_Print(T, L) BSQON_NLIST_##T##Print(L)

////////////////////////////
//AST Declaration Macros

#define BSQON_AST_NODE(T) BSQON_AST_NODE_##T

#define BSQON_AST_NODE_DECLARE_0(T) \
struct BSQON_AST_NODE(T) { \
    struct BSQON_AST_Node base; \
}; \
const struct BSQON_AST_NODE(T)* BSQON_AST_##T##As(const struct BSQON_AST_Node* node); \
struct BSQON_AST_Node* BSQON_AST_##T##Create(enum BSQON_AST_TAG tag, struct AST_SourcePos sl);

#define BSQON_AST_NODE_DECLARE_1(T, FTYPE1, FNAME1) \
struct BSQON_AST_NODE(T) { \
    struct BSQON_AST_Node base; \
    FTYPE1 FNAME1; \
}; \
const struct BSQON_AST_NODE(T)* BSQON_AST_##T##As(const struct BSQON_AST_Node* node); \
struct BSQON_AST_Node* BSQON_AST_##T##Create(enum BSQON_AST_TAG tag, struct AST_SourcePos sl, FTYPE1 FNAME1);

#define BSQON_AST_NODE_DECLARE_2(T, FTYPE1, FNAME1, FTYPE2, FNAME2) \
struct BSQON_AST_NODE(T) { \
    struct BSQON_AST_Node base; \
    FTYPE1 FNAME1; \
    FTYPE2 FNAME2; \
}; \
const struct BSQON_AST_NODE(T)* BSQON_AST_##T##As(const struct BSQON_AST_Node* node); \
struct BSQON_AST_Node* BSQON_AST_##T##Create(enum BSQON_AST_TAG tag, struct AST_SourcePos sl, FTYPE1 FNAME1, FTYPE2 FNAME2);

#define BSQON_AST_NODE_DECLARE_3(T, FTYPE1, FNAME1, FTYPE2, FNAME2, FTYPE3, FNAME3) \
struct BSQON_AST_NODE(T) { \
    struct BSQON_AST_Node base; \
    FTYPE1 FNAME1; \
    FTYPE2 FNAME2; \
    FTYPE3 FNAME3; \
}; \
const struct BSQON_AST_NODE(T)* BSQON_AST_##T##As(const struct BSQON_AST_Node* node); \
struct BSQON_AST_Node* BSQON_AST_##T##Create(enum BSQON_AST_TAG tag, struct AST_SourcePos sl, FTYPE1 FNAME1, FTYPE2 FNAME2, FTYPE3 FNAME3);

#define BSQON_AST_NODE_DECLARE_4(T, FTYPE1, FNAME1, FTYPE2, FNAME2, FTYPE3, FNAME3, FTYPE4, FNAME4) \
struct BSQON_AST_NODE(T) { \
    struct BSQON_AST_Node base; \
    FTYPE1 FNAME1; \
    FTYPE2 FNAME2; \
    FTYPE3 FNAME3; \
    FTYPE4 FNAME4; \
}; \
const struct BSQON_AST_NODE(T)* BSQON_AST_##T##As(const struct BSQON_AST_Node* node); \
struct BSQON_AST_Node* BSQON_AST_##T##Create(enum BSQON_AST_TAG tag, struct AST_SourcePos sl, FTYPE1 FNAME1, FTYPE2 FNAME2, FTYPE3 FNAME3, FTYPE4 FNAME4);

////////////////////////////
//AST Definition Macros

#define BSQON_AST_NODE_AS(T, N) BSQON_AST_##T##As(N)
#define BSQON_AST_NODE_CONS(T, TAG, SL, ...) BSQON_AST_##T##Create(TAG, SL __VA_OPT__(,) __VA_ARGS__)
#define BSQON_AST_NODE_PRINT(T, N) BSQON_AST_##T##Print(BSQON_AST_NODE_AS(T, N))

#define BSQON_AST_LIST_DEFINE(T) \
struct BSQON_LIST(T)* BSQON_LIST_##T##Singleton(struct T* value) \
{ \
    struct BSQON_LIST(T)* res = AST_ALLOC(sizeof(struct BSQON_LIST(T))); \
    res->value = value; \
    res->next = NULL; \
    return res; \
} \
struct BSQON_LIST(T)* BSQON_LIST_##T##Push(struct T* value, struct BSQON_LIST(T)* ll) \
{ \
    struct BSQON_LIST(T)* res = AST_ALLOC(sizeof(struct BSQON_LIST(T))); \
    res->value = value; \
    res->next = ll; \
    return res; \
} \
struct BSQON_LIST(T)* BSQON_LIST_##T##Reverse(struct BSQON_LIST(T)* ll) \
{ \
    struct BSQON_LIST(T)* lp = NULL; \
    while(ll != NULL) { \
        struct BSQON_LIST(T)* lc = ll; \
        ll = ll->next; \
\
        lc->next = lp; \
        lp = lc; \
    } \
\
    return lp; \
} \
void BSQON_LIST_##T##Print(struct BSQON_LIST(T)* list) \
{ \
    for(struct BSQON_LIST(T)* ll = list; ll != NULL; ll = ll->next) \
    { \
        BSQON_AST_print(ll->value); \
        if(ll->next != NULL) { \
            printf(", "); \
        } \
    } \
}

#define BSQON_AST_NLIST_DEFINE(T) \
struct BSQON_NLIST_ENTRY(T) BSQON_NLIST_##T##Create(const char* name, struct T* value) \
{\
    struct BSQON_NLIST_ENTRY(T) res;\
    res.name = name;\
    res.value = value;\
    return res;\
}\
struct BSQON_NLIST(T)* BSQON_NLIST_##T##Singleton(struct BSQON_NLIST_ENTRY(T) entry) \
{ \
    struct BSQON_NLIST(T)* res = AST_ALLOC(sizeof(struct BSQON_NLIST(T))); \
    res->entry = entry; \
    res->next = NULL; \
    return res; \
} \
struct BSQON_NLIST(T)* BSQON_NLIST_##T##Push(struct BSQON_NLIST_ENTRY(T) entry, struct BSQON_NLIST(T)* ll) \
{ \
    struct BSQON_NLIST(T)* res = AST_ALLOC(sizeof(struct BSQON_NLIST(T))); \
    res->entry = entry; \
    res->next = ll; \
    return res; \
} \
struct BSQON_NLIST(T)* BSQON_NLIST_##T##Reverse(struct BSQON_NLIST(T)* ll) \
{ \
    struct BSQON_NLIST(T)* lp = NULL; \
    while(ll != NULL) { \
        struct BSQON_NLIST(T)* lc = ll; \
        ll = ll->next; \
\
        lc->next = lp; \
        lp = lc; \
    } \
\
    return lp; \
} \
void BSQON_NLIST_##T##Print(struct BSQON_NLIST(T)* list) \
{ \
    for(struct BSQON_NLIST(T)* ll = list; ll != NULL; ll = ll->next) \
    { \
        printf("%s: ", ll->entry.name != NULL ? ll->entry.name : "_"); \
        BSQON_AST_print(ll->entry.value); \
        if(ll->next != NULL) { \
            printf(", "); \
        } \
    } \
}

#define BSQON_AST_NODE_DEFINE_0(T) \
const struct BSQON_AST_NODE(T)* BSQON_AST_##T##As(const struct BSQON_AST_Node* node) { return (const struct BSQON_AST_NODE(T)*)node; } \
struct BSQON_AST_Node* BSQON_AST_##T##Create(enum BSQON_AST_TAG tag, struct AST_SourcePos sl) \
{ \
    struct BSQON_AST_NODE(T)* node = (struct BSQON_AST_NODE(T)*)AST_ALLOC(sizeof(struct BSQON_AST_NODE(T))); \
    node->base.tag = tag; \
    node->base.pos = sl; \
    return (struct BSQON_AST_Node*)node; \
}

#define BSQON_AST_NODE_DEFINE_1(T, FTYPE1, FNAME1) \
const struct BSQON_AST_NODE(T)* BSQON_AST_##T##As(const struct BSQON_AST_Node* node) { return (const struct BSQON_AST_NODE(T)*)node; } \
struct BSQON_AST_Node* BSQON_AST_##T##Create(enum BSQON_AST_TAG tag, struct AST_SourcePos sl, FTYPE1 FNAME1) \
{ \
    struct BSQON_AST_NODE(T)* node = (struct BSQON_AST_NODE(T)*)AST_ALLOC(sizeof(struct BSQON_AST_NODE(T))); \
    node->base.tag = tag; \
    node->base.pos = sl; \
    node->FNAME1 = FNAME1; \
    return (struct BSQON_AST_Node*)node; \
}

#define BSQON_AST_NODE_DEFINE_2(T, FTYPE1, FNAME1, FTYPE2, FNAME2) \
const struct BSQON_AST_NODE(T)* BSQON_AST_##T##As(const struct BSQON_AST_Node* node) { return (const struct BSQON_AST_NODE(T)*)node; } \
struct BSQON_AST_Node* BSQON_AST_##T##Create(enum BSQON_AST_TAG tag, struct AST_SourcePos sl, FTYPE1 FNAME1, FTYPE2 FNAME2) \
{ \
    struct BSQON_AST_NODE(T)* node = (struct BSQON_AST_NODE(T)*)AST_ALLOC(sizeof(struct BSQON_AST_NODE(T))); \
    node->base.tag = tag; \
    node->base.pos = sl; \
    node->FNAME1 = FNAME1; \
    node->FNAME2 = FNAME2; \
    return (struct BSQON_AST_Node*)node; \
}

#define BSQON_AST_NODE_DEFINE_3(T, FTYPE1, FNAME1, FTYPE2, FNAME2, FTYPE3, FNAME3) \
const struct BSQON_AST_NODE(T)* BSQON_AST_##T##As(const struct BSQON_AST_Node* node) { return (const struct BSQON_AST_NODE(T)*)node; } \
struct BSQON_AST_Node* BSQON_AST_##T##Create(enum BSQON_AST_TAG tag, struct AST_SourcePos sl, FTYPE1 FNAME1, FTYPE2 FNAME2, FTYPE3 FNAME3) \
{ \
    struct BSQON_AST_NODE(T)* node = (struct BSQON_AST_NODE(T)*)AST_ALLOC(sizeof(struct BSQON_AST_NODE(T))); \
    node->base.tag = tag; \
    node->base.pos = sl; \
    node->FNAME1 = FNAME1; \
    node->FNAME2 = FNAME2; \
    node->FNAME3 = FNAME3; \
    return (struct BSQON_AST_Node*)node; \
}

#define BSQON_AST_NODE_DEFINE_4(T, FTYPE1, FNAME1, FTYPE2, FNAME2, FTYPE3, FNAME3, FTYPE4, FNAME4) \
const struct BSQON_AST_NODE(T)* BSQON_AST_##T##As(const struct BSQON_AST_Node* node) { return (const struct BSQON_AST_NODE(T)*)node; } \
struct BSQON_AST_Node* BSQON_AST_##T##Create(enum BSQON_AST_TAG tag, struct AST_SourcePos sl, FTYPE1 FNAME1, FTYPE2 FNAME2, FTYPE3 FNAME3, FTYPE4 FNAME4) \
{ \
    struct BSQON_AST_NODE(T)* node = (struct BSQON_AST_NODE(T)*)AST_ALLOC(sizeof(struct BSQON_AST_NODE(T))); \
    node->base.tag = tag; \
    node->base.pos = sl; \
    node->FNAME1 = FNAME1; \
    node->FNAME2 = FNAME2; \
    node->FNAME3 = FNAME3; \
    node->FNAME4 = FNAME4; \
    return (struct BSQON_AST_Node*)node; \
}

