#ifndef SC_DATA_H
#define SC_DATA_H

/* Init the linked list for data */
void init_mididata();

/* Get a pointer to the mididata. Int supplied by reference as argument is 
   updated with item count */

char* get_mididata(int *count);

#endif /* SC_DATA_H */
