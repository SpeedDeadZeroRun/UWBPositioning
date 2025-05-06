#include<stdio.h>
#include"parser.h"

int main() {
    struct Data *d = init_data();
    char *dataChr = "00:0A 10:01 4520 -97.57,00:0A 10:02 3100 -87.59,00:0B 10:03 21500 -120.35,";
    int end = parse(d, dataChr, 0, 74);
    printf("%s %s %.0f %.2f\n",d->anchor_ip, d->tag_ip, d->distance, d->rssi);
    end = parse(d, dataChr, end, 74);
    printf("%s %s %.0f %.2f\n",d->anchor_ip, d->tag_ip, d->distance, d->rssi);
    end = parse(d, dataChr, end, 74);
    printf("%s %s %.0f %.2f\n",d->anchor_ip, d->tag_ip, d->distance, d->rssi);
    printf("%d\n", end);

    free(d);

    return 0;
}
