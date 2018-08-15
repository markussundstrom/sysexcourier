

#include <ncurses.h>
#include <string.h>
#include "sc_midi.h"


int main () {
    WINDOW *dataentry;
    int height, width;
    init_jack();
    initscr();
    start_color();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    init_pair(1, COLOR_YELLOW, COLOR_BLUE);
    wbkgd(stdscr, COLOR_PAIR(1));
    attron(A_BOLD);
    mvprintw(0, (COLS/2)-6, "SysExCourier");
    attroff(A_BOLD);
    refresh();
    height = LINES - 4;
    width = COLS - 4;
    dataentry = newwin(height, width, 1, 2);
    box(dataentry, ACS_VLINE, ACS_HLINE);
    wrefresh(dataentry);
    getch();
    endwin();

    return 0;
} 
