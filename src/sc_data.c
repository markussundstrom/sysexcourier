#include <stdlib.h>
#include "sc_data.h"

typedef struct midibyte {
    struct midibyte *next;
    struct midibyte *prev;
    unsigned char data;
}midibyte;

midibyte *databytes;

void init_data() {
    databytes = calloc(1, sizeof(midibyte*));
    databytes->next = NULL;
    databytes->prev = NULL;
    databytes->data = 0x00; 
}
