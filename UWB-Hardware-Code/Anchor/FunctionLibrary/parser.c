#include<stdlib.h>
#include<string.h>

struct Command {
    char sel[8];
    char arg1[8];
    char arg2[8];
    char arg3[8];
};

void clear_command(struct Command *c) {
    memset(c->sel, '\0', sizeof(char)*8);
    memset(c->arg1, '\0', sizeof(char)*8);
    memset(c->arg2, '\0', sizeof(char)*8);
    memset(c->arg3, '\0', sizeof(char)*8);
}

struct Command *init_command() {
    struct Command *c = (struct Command *)malloc(sizeof(struct Command));
    clear_command(c);

    return c;
}

void parse(struct Command *c, const char *input, int string_length) {
    clear_command(c);
    int start_pivot = 0;
    int end_pivot = 0;
    for(int i = 0; i < 4; i++) {
        while(end_pivot < string_length) {
            if(
                (*(input+end_pivot) == ' ') ||
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
            case 0:strncpy(c->sel, temp, 8); break;
            case 1:strncpy(c->arg1, temp, 8); break;
            case 2:strncpy(c->arg2, temp, 8); break;
            case 3:strncpy(c->arg3, temp, 8); break;
            default:break;
        }
        if(end_pivot >= string_length-1) return;
        start_pivot = end_pivot+1;
        end_pivot = start_pivot;
    }
}
