#ifndef parser
#define parser

#ifdef __cplusplus
extern "C" {
#endif

#include<stdlib.h>
#include<string.h>
#include"parser.c"

struct Data;
struct Data *init_data();
int parse(struct Data *d, const char *input, int start, int string_length);

#ifdef __cplusplus
}
#endif

#endif
