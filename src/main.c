

#include <ncurses.h>

int main () {
    WINDOW *dataentry;
    int height, width;
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    height = LINES - 4;
    width = COLS - 4;
    dataentry = newwin(height, width, 1, 2);
    box(dataentry, ACS_VLINE, ACS_HLINE);
    refresh();
    wrefresh(dataentry);
    getch();
    endwin();

    return 0;
} 
