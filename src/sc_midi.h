struct midibyte {
    jack_midi_data_t data;
    struct midibyte *prev;
    struct midibyte *next;
};
