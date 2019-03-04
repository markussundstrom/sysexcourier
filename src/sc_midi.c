
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <jack/jack.h>
#include <jack/midiport.h>
#include "sc_midi.h"


struct midibyte {
    struct midibyte *next;
    struct midibyte *prev;
    jack_midi_data_t data;
};

jack_client_t *sc_jack_client;
jack_port_t *midi_in;
jack_port_t *midi_out;


void init_client() {
    sc_jack_client = jack_client_open("SysExCourier", JackNullOption, NULL);
    if (sc_jack_client == NULL) {
        fprintf(stderr, "Unable to initialize JACK client");
        exit(1);
    }
    midi_in = jack_port_register(sc_jack_client, "MIDI in",
                                 JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 
                                 0);
    if (midi_in == NULL) {
        fprintf(stderr, "Unable to create input port");
        exit(1);
    }
    midi_out = jack_port_register(sc_jack_client, "MIDI out",
                                 JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput,
                                 0);
    if (midi_in == NULL) {
        fprintf(stderr, "Unable to create output port");
        exit(1);
    }
}

char** list_ports(int *count, portdirection dir) {
    char const **ports;
    char  **returnedports;
    size_t portnamelength; 
    int i;

    portnamelength = jack_port_name_size();
    ports = jack_get_ports(sc_jack_client, NULL, JACK_DEFAULT_MIDI_TYPE, 
                           (dir == inputport) ? 
                            JackPortIsInput : JackPortIsOutput);
    *count = 0;
    while(ports[*count] != NULL) {
        (*count)++;
    }
    returnedports = calloc((*count), sizeof (char*));

    for(i = 0; i < *count; i++) {
        returnedports[i] = calloc(portnamelength, sizeof(char));
        strcpy(returnedports[i], ports[i]);
    } 

    jack_free(ports);
    return returnedports;
}



void shutdown_client() {
    jack_port_unregister(sc_jack_client, midi_in);
    jack_port_unregister(sc_jack_client, midi_out);
    jack_client_close(sc_jack_client);
}


