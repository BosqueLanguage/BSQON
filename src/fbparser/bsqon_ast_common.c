#include "bsqon_ast_common.h"

struct AST_SourcePos createSourcePos(uint32_t first_line, uint32_t first_column, uint32_t last_line, uint32_t last_column)
{
    struct AST_SourcePos res;
    res.first_line = first_line;
    res.first_column = first_column;
    res.last_line = last_line;
    res.last_column = last_column;
    return res;
}
