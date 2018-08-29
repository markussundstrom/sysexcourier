all: sysexcourier

sources = main.c sc_midi.c

sysexcourier: $(sources)
	gcc main.c sc_midi.c -o $@ -ljack -lcurses
	
clean:
	-rm sysexcourier

.PHONY: all clean
