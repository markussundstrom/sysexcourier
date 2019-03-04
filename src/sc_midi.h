#ifndef SC_MIDI_H
#define SC_MIDI_H
struct midibyte;

typedef enum {inputport, outputport} portdirection; 

void init_client();
void shutdown_client();
char ** list_ports(int *count, portdirection dir);

#endif /* SC_MIDI_H */
