#include<stdio.h>
#include"parser.h"

int main() {
    struct Command *c = init_command();
    char *dataChr = "sel set name A\n";
    parse(c, dataChr, 15);
    printf("%s %s %s %s\n", c->sel, c->arg1, c->arg2, c->arg3);
    dataChr = "all set start\n";
    parse(c, dataChr, 14);
    printf("%s %s %s %s\n", c->sel, c->arg1, c->arg2, c->arg3);
    dataChr = "all set led on\n";
    parse(c, dataChr, 15);
    printf("%s %s %s %s\n", c->sel, c->arg1, c->arg2, c->arg3);
    dataChr = "all set led off\n";
    parse(c, dataChr, 16);
    printf("%s %s %s %s\n", c->sel, c->arg1, c->arg2, c->arg3);

    free(c);

    return 0;
}
