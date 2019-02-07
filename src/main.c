

#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <menu.h>
#include "sc_midi.h"

void ports_menu();

int main () {
    WINDOW *dataentry;
    int height, width, running;
    init_client();
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
    running = 1;
    while (running) {
        int keypress;
        keypress = getch();
        switch (keypress) {
            case KEY_F(12):
                running = 0;
                break;
            case KEY_F(5):
                ports_menu();
                break;
            default:
                break;
        }
    }
    shutdown_client();
    endwin();

    return 0;
}

void ports_menu() {
    char **ports;
    ITEM **menuitems;
    MENU *portsmenu;
    int count, key, i;
    char buffer[10];

    ports = list_ports(&count);
    menuitems = (ITEM **)calloc(count + 1, sizeof(ITEM *));
    for (i = 0; i < count; i++) {
        menuitems[i] = new_item(ports[i], ports[i]);
    }
    menuitems[count] = (ITEM *)NULL;
    portsmenu = new_menu((ITEM **)menuitems);
    post_menu(portsmenu);

    while ((key = getch()) != KEY_F(1)) {
        switch(key) {
            case KEY_DOWN:
                menu_driver(portsmenu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(portsmenu, REQ_UP_ITEM);
                break;
        }
    }
    for (i = 0; i <= count; i++) {
        free_item(menuitems[i]);
        free(ports[i]);
    }
    free_item(menuitems[count]);
    free_menu(portsmenu);
    free(ports);
}


        


