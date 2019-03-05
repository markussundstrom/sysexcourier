

#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <menu.h>
#include "sc_midi.h"

/*Construct a menu listing available jack ports, in or out, and await selection*/
void ports_menu(portdirection dir);
/*Refresh all elements of main screen*/
void refresh_sc_screen();


WINDOW *dataentry, *rxportwin, *txportwin;

int main () {
    int height, width, running;
    init_client();
    initscr();
    start_color();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    init_pair(1, COLOR_YELLOW, COLOR_BLUE);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    wbkgd(stdscr, COLOR_PAIR(1));
    attron(A_BOLD);
    mvprintw(0, (COLS/2)-6, "SysExCourier");
    attroff(A_BOLD);
    refresh();
    height = LINES - 6;
    width = COLS - 4;
    dataentry = newwin(height, width, 1, 2);
    box(dataentry, ACS_VLINE, ACS_HLINE);
    wrefresh(dataentry);
    rxportwin = newwin(1, (COLS/2)-4, LINES-3  , 2);
    wbkgd(rxportwin, COLOR_PAIR(2));
    wrefresh(rxportwin);
    txportwin = newwin(1, (COLS/2)-4, LINES-3, (COLS/2)+2);
    wbkgd(txportwin, COLOR_PAIR(2));
    wrefresh(txportwin);
    attron(A_BOLD);
    mvprintw(LINES-4, 2, "Receive from");
    mvprintw(LINES-4, (COLS/2)+2, "Send to");
    attroff(A_BOLD);

    running = 1;
    while (running) {
        int keypress;
        keypress = getch();
        switch (keypress) {
            case KEY_F(12):
                running = 0;
                break;
            case KEY_F(5):
                ports_menu(rxport);
                refresh_sc_screen();
                break;
            case KEY_F(6):
                ports_menu(txport);
                refresh_sc_screen();
            default:
                break;
        }
    }
    shutdown_client();
    endwin();

    return 0;
}


void ports_menu(portdirection dir) {
    char **ports;
    ITEM **menuitems;
    MENU *portsmenu;
    WINDOW *menuwindow;
    int count, key, i, begx, maxx, menu;
    
    /*Get list of ports and setup menu contents*/ 
    ports = list_ports(&count, dir);
    menuitems = (ITEM **)calloc(count + 1, sizeof(ITEM *));
    for (i = 0; i < count; i++) {
        menuitems[i] = new_item(ports[i], NULL);
    }
    menuitems[count] = (ITEM *)NULL;
    portsmenu = new_menu((ITEM **)menuitems);
    begx = getbegx((dir == rxport) ? rxportwin : txportwin);
    maxx = getmaxx((dir == rxport) ? rxportwin : txportwin);
    menuwindow = newwin(LINES-3, maxx, 1, begx);
    set_menu_win(portsmenu, menuwindow);
    post_menu(portsmenu);
    wrefresh(menuwindow);
    
    menu = 1;
    while (menu) {
        key = getch();
        switch(key) {
            case KEY_F(1):
                menu = 0;
                break;
            case KEY_DOWN:
                menu_driver(portsmenu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(portsmenu, REQ_UP_ITEM);
                break;
            case 10:
                connect_port(dir, item_name(current_item(portsmenu)));
                menu = 0;
                break;
        }
        wrefresh(menuwindow);
    }

    /*cleanup menu*/
    unpost_menu(portsmenu);
    free_menu(portsmenu);
    for (i = 0; i < count; i++) {
        free_item(menuitems[i]);
        free(ports[i]);
    }
    free_item(menuitems[count]);
    free(ports);
    delwin(menuwindow);
}

void refresh_sc_screen() {
    touchwin(stdscr);
    touchwin(dataentry);
    touchwin(txportwin);
    touchwin(rxportwin);
    wnoutrefresh(stdscr);
    wnoutrefresh(dataentry);
    wnoutrefresh(txportwin);
    wnoutrefresh(rxportwin);
    doupdate();
}
