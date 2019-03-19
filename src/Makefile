all: sysexcourier

sources = main.c sc_midi.c sc_data.c

sysexcourier: $(sources)
	gcc main.c sc_midi.c sc_data.c -o $@ -ljack -lcurses -lmenu
	
clean:
	-rm sysexcourier

.PHONY: all clean
