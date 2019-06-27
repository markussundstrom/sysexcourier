

#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <menu.h>
#include "sc_midi.h"
#include "sc_data.h"

/*Construct a menu listing available jack ports, in or out, and await selection*/
void ports_menu(portdirection dir);
/*Refresh all elements of main screen*/
void refresh_sc_screen();

/*refresh dataentry contents*/
void refresh_dataentry();


WINDOW *dataentry, *rxportwin, *txportwin;
int entryoffset;

int main () {
    int height, width, running;
    init_client();
    init_mididata();
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
    entryoffset = 0;
    running = 1;
    refresh_sc_screen();
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
    int count;
    char **connections;
    int i;
    
    count = 0;
    touchwin(stdscr);
/*    touchwin(dataentry);*/
    werase(rxportwin);
    werase(txportwin);
    connections = get_connections(&count, rxport);
    if(count > 0) {
        /*GUI only designed for displaying one connection. Displaying first
          connection and throwing away the rest*/
        mvwprintw(rxportwin, 0, 0, connections[0]);
        for(i = 0; i < count; i++) {
            free(connections[i]);
        }
        free(connections);
        
    } else {
        wmove(rxportwin, 0, 0);
        clrtoeol();
    }
    connections = get_connections(&count, txport);
    if(count > 0) {
        /*GUI only designed for displaying one connection. Displaying first
          connection and throwing away the rest*/
        mvwprintw(txportwin, 0, 0, connections[0]);
        for(i = 0; i < count; i++) {
            free(connections[i]);
        }
        free(connections);
    }
    touchwin(rxportwin);
    touchwin(txportwin);
    wnoutrefresh(stdscr);
/*    wnoutrefresh(dataentry);*/
    wnoutrefresh(txportwin);
    wnoutrefresh(rxportwin);
    refresh_dataentry();
    doupdate();
}

void refresh_dataentry() {
    unsigned char *data;
    int count;
    int i, x, y;

    count = 0;
    data = get_mididata(&count);
    wclear(dataentry);
    x = y = 0;
    wmove(dataentry, y, x);
    if (data != NULL) {
        for (i = 0; i < count; i++) {
            if (i == entryoffset) {
                wattron(dataentry, A_REVERSE);
            } else {
                wattroff(dataentry, A_REVERSE);
            }
            mvwprintw (dataentry, y, x, "%.2X", data[i]);
            /*Is there enough room for displaying another byte on this line, 
            factoring in window borders?*/
            /*FIXME Scrollbar?*/
            if (x >= LINES - 4 - 2) {
                x = 0;
                y++;
            } else {
                x += 3;
            }
            /*wmove(dataentry, y, x);*/
        }
        if (entryoffset < count) {
            wattroff(dataentry, A_REVERSE);
        } else {
            wattron(dataentry, A_REVERSE);
        }

        mvwprintw(dataentry, y, x, "--");
        /*touchwin(dataentry);*/
        wrefresh(dataentry);
    }
}
