/* @file clients_file.h
 @brief Client persistence handling (binary file).
 This module is responsible for saving and loading clients
 from a binary file. It does not perform validation or
 business logic.
 The file format is:
- First: integer representing the number of clients
- Then: array of Client structures */
#ifndef CLIENTS_FILE_H
#define CLIENTS_FILE_H

#include "client.h"

/* Loads clients from a binary file.
 Reads the number of clients first and then loads
 the clients array into dynamically allocated memory.
 @param filename Path to the file
 @param count    Pointer to store the number of clients loaded
 @return Pointer to the loaded clients array, or NULL if file does not exist */
Client* load_clients(const char* filename, int* count);

/* Saves clients to a binary file.
 Writes the number of clients followed by the
 clients array into the file.
 @param filename Path to the file
 @param clients  Clients array
 @param count    Number of clients*/

void save_clients(const char* filename, Client* clients, int count);

#endif




