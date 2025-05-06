#ifndef link
#define link

#ifdef __cplusplus
extern "C" {
#endif

#include<stdint.h>
#include<stdlib.h>
#include"link.c"

struct MyLink *init_link();
void add_link(struct MyLink *p, uint16_t addr);
struct MyLink *find_link(struct MyLink *p, uint16_t addr);
void fresh_link(struct MyLink *p, uint16_t addr, float range, float dbm);
void delete_link(struct MyLink *p, uint16_t addr);
float get_avg_range(struct MyLink *p);
float get_avg_rssi(struct MyLink *p);

#ifdef __cplusplus
}
#endif

#endif
