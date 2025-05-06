#ifndef parser
#define parser

#ifdef __cplusplus
extern "C" {
#endif

#include<stdlib.h>
#include<string.h>
#include"parser.c"

struct Command;
struct Command *init_command();
void parse(struct Command *c, const char *input, int string_length);

#ifdef __cplusplus
}
#endif

#endif
