#include "smt_utils.h"
#include <cstdio>
#include <cstring>

void badArgs(const char* msg)
{
    const char* usage = "USAGE: smtextract <formula.smt2> <fn_signature.json> <assembly.json> --<MODE>\nMODES:\n"
                        "\t-e|--extract\t- Extract Err Values from SMT\n"
                        "\t-g|--generate\t- Generate Test Values from SMT";

    printf("%s\n", usage);
    printf("%s\n", msg);
    exit(1);
}

bool validPath(const char* filepath, const char* extension)
{
    char* og;
    char* tokens;
    char* track;
    bool valid = false;

    og = strdup(filepath);
    track = og;

    while((tokens = strtok_r(track, ".", &track))) {
        if(strcmp(tokens, extension) == 0) {
            valid = true;
        }
    }

    return valid;
}
