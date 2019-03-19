#ifndef SC_MIDI_H
#define SC_MIDI_H

/*Enumeration used to indicate whether a midiport is receiving or transmitting.
This enumeration is always to be used from the pov of this application, ie
rxport always means the receiving port of this application, never the direction
of any other Jack applications ports.*/
typedef enum {rxport, txport} portdirection; 

void init_client();
char ** list_ports(int *count, portdirection dir);
void connect_port(portdirection dir, const char *port);
char ** get_connections(int *count, portdirection dir);
void shutdown_client();

#endif /* SC_MIDI_H */
