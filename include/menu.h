/* @file menu.h
* @brief Generic menu utilities for the system.
* This module provides functions to display menus and
* read user options. It contains no business logic and
 is shared across different modules (clients, products, etc.) */
#ifndef MENU_H
#define MENU_H


//Displays the main system menu.
void show_main_menu(void);

//Displays the clients management menu.
void show_clients_menu(void);

// Reads an integer option from standard input.
// @return The selected menu option
int read_option(void);


int read_option_in_range(int min, int max);

#endif

