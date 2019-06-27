#include <stdlib.h>
#include "sc_data.h"

typedef struct midibyte {
    struct midibyte *next;
    struct midibyte *prev;
    unsigned char data;
}midibyte;

midibyte *databytes;

void init_mididata() {
    databytes = calloc(1, sizeof(midibyte*));
    databytes->next = NULL;
    databytes->prev = NULL;
    databytes->data = 0x00;
}

char* get_mididata(int *count) {
    char *bytestream;
    int allocation;
    midibyte *current;

    *count = 0;

    if (databytes == NULL) {
        return NULL;
    } else {
        allocation = 100;
        bytestream = calloc(allocation, sizeof(char));
        current = databytes;
        while (current != NULL) {
            bytestream[*count] = current->data;
            current = current->next;
            (*count)++;
            if (*count >= allocation) {
                allocation += allocation * 1.5;
                bytestream = (char*) realloc(bytestream, allocation);
            }
        }
        return bytestream;
    }
}



