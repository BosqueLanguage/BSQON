#pragma once

#include "bsqon_ast_common.h"

#ifdef __cplusplus
extern "C" 
{
#endif

struct ByteString
{
    uint8_t* bytes;
    uint64_t len;
};

struct ByteString* bstrAlloc(struct ByteString dst);

#ifdef __cplusplus
}
#endif
