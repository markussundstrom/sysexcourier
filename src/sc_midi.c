
#include <stdio.h>
#include <jack/jack.h>
#include <jack/midiport.h>


struct midibyte {
    struct midibyte *next;
    struct midibyte *prev;
    jack_midi_data_t data;
};

jack_client_t *sc_jack_client;

void init_jack() {
    sc_jack_client = jack_client_open("SysExCourier", JackNullOption, NULL);
    if (sc_jack_client == NULL) {
        fprintf(stderr, "Unable to initialize JACK client");
        exit(1);
    }
}


