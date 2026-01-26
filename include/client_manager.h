#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include "client.h"

// CRUD//
Client* add_client(Client* clients, int* count, int id, const char* name);

Client* remove_client_by_id(Client* clients, int* count, int id);

int edit_client ( Client* clients, int count, int id , const char* new_name);


//utils//
void print_all_clients(Client* clients, int count);

void free_all_clients(Client* clients, int count);


// Search//
int find_client_index_by_id(const Client* clients, int count, int id);


//Checks if a client ID already exists//
int client_id_exists(const Client* clients, int count, int id);




#endif


