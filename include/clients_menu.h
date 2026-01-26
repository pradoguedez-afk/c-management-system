/* @file clients_menu.h
 @brief User interface menu for client operations.
This module handles user interaction related to clients.
It displays menu options and delegates actions to the
client_manager module.
It does not implement business logic or file operations */
#ifndef CLIENTS_MENU_H
#define CLIENTS_MENU_H

#include "client.h"


// Displays the clients menu and handles user choices.
// Allows the user to add, remove, list, and edit clients
// by calling the appropriate client_manager functions.
// @param clients Pointer to the clients array
// @param count   Pointer to the number of clients.

void clients_menu(Client** clients, int* count);

#endif


