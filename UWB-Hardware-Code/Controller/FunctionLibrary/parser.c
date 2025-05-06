#include<stdlib.h>
#include<string.h>

struct Data {
    char anchor_ip[8];
    char tag_ip[8];
    float distance;
    float rssi;
};

void clear_data(struct Data *d) {
    memset(d->anchor_ip, '\0', sizeof(char)*8);
    memset(d->tag_ip, '\0', sizeof(char)*8);
    d->distance = 0.0;
    d->rssi = 0.0;
}

struct Data *init_data() {
    struct Data *d = (struct Data *)malloc(sizeof(struct Data));
    clear_data(d);

    return d;
}

int parse(struct Data *d, const char *input, int start, int string_length) {
    clear_data(d);
    int start_pivot = start;
    int end_pivot = start;
    for(int i = 0; i < 4; i++) {
        while(end_pivot < string_length) {
            if(
                (*(input+end_pivot) == ' ') ||
                (*(input+end_pivot) == ',') ||
                (*(input+end_pivot) == '\n')
            ) {
                break;
            }
            end_pivot = end_pivot+1;
        }
        char temp[8];
        memset(temp, '\0', sizeof(char)*8);
        strncpy(temp, input+start_pivot, end_pivot-start_pivot);
        switch (i) {
            case 0:strncpy(d->anchor_ip, temp, 8); break;
            case 1:strncpy(d->tag_ip, temp, 8); break;
            case 2:d->distance = atof(temp); break;
            case 3:d->rssi= atof(temp); break;
            default:break;
        }
        if(end_pivot >= string_length-1) return -1;
        start_pivot = end_pivot+1;
        end_pivot = start_pivot;
    }

    return start_pivot;
}
