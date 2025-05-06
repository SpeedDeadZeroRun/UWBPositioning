#include<stdint.h>
#include<stdlib.h>

#define arr_size 40

int need_to_wait_for_other_task = 0;

struct MyLink {
    uint16_t address;
    float range[arr_size];
    float rssi[arr_size];
    int curser;
    struct MyLink *next;
    unsigned long last_seen;
};

struct MyLink *init_link() {
    struct MyLink *p = (struct MyLink *)malloc(sizeof(struct MyLink));
    p->next = NULL;
    p->address = 0;
    p->curser = 0;
    p->last_seen = millis();
    for(int i = 0; i < arr_size; i++) {
        p->range[i] = 0.0;
        p->rssi[i] = 0.0;
    }
    return p;
}

void wait_for_other_task() {
    while(need_to_wait_for_other_task);
    need_to_wait_for_other_task = 1;
}

void signal_for_other_task() {
    need_to_wait_for_other_task = 0;
}

void add_link(struct MyLink *p, uint16_t addr) {
    wait_for_other_task();
    struct MyLink *temp = p;
    while(temp->next != NULL) temp = temp->next;
    struct MyLink *a = init_link();
    a->address = addr;
    temp->next = a;
    signal_for_other_task();
    return;
}

struct MyLink *find_link(struct MyLink *p, uint16_t addr) {
    if (addr == 0) return NULL;
    wait_for_other_task();
    struct MyLink *temp = p;
    while (temp->next != NULL) {
        temp = temp->next;
        if (temp->address == addr) return temp;
    }
    signal_for_other_task();
    return NULL;
}

void fresh_link(struct MyLink *p, uint16_t addr, float range, float rssi) {
    struct MyLink *temp = find_link(p, addr);
    wait_for_other_task();
    if (temp != NULL) {
        temp->range[temp->curser] = range;
        temp->rssi[temp->curser] = rssi;
        temp->last_seen = millis();
        temp->curser = (temp->curser+1)%arr_size;
    }
    signal_for_other_task();
    return;
}

void delete_link(struct MyLink *p, uint16_t addr) {
    if (addr == 0) return;
    wait_for_other_task();
    struct MyLink *temp = p;
    while (temp->next != NULL) {
        if (temp->next->address == addr) {
            struct MyLink *del = temp->next;
            temp->next = del->next;
            free(del);
            del = NULL;
            return;
        }
        temp = temp->next;
    }
    signal_for_other_task();
    return;
}

float get_avg_range(struct MyLink *p) {
    float avg_range = 0.0;
    wait_for_other_task();
    if(p == NULL) return 0.0;
    for(int i = 0; i < arr_size; i++) avg_range = avg_range+p->range[i];
    avg_range = avg_range/arr_size;
    signal_for_other_task();
    return avg_range;
}

float get_avg_rssi(struct MyLink *p) {
    float avg_rssi = 0.0;
    wait_for_other_task();
    if(p == NULL) return 0.0;
    for(int i = 0; i < arr_size; i++) avg_rssi = avg_rssi+p->rssi[i];
    avg_rssi = avg_rssi/arr_size;
    signal_for_other_task();
    return avg_rssi;
}