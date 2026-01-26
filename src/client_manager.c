/*This module handles all operations related to clients,
including creation, deletion, search, update, and memory management.
It does not handle user input or file persistence.*/
#include <stdio.h>
#include <stdlib.h>
#include "client_manager.h"
#include <string.h>

#define CLIENT_FILE "data/clients.dat"



// ================= ADD ================= //
/* Adds a new client to the dynamic list.
 Reallocates memory to store one more client and updates the count.
@param clients Pointer to the current clients array
@param count   Pointer to the number of clients
@param id      Client identifier
@param name    Client name
@return Updated clients array (may change due to realloc)*/

Client* add_client(Client* clients, int* count, int id, const char* name)
{
    Client* temp = realloc(clients, sizeof(Client) * (*count + 1));
    if (!temp) return clients;

    clients = temp;
    clients[*count].id = id;
    strcpy(clients[*count].name, name);
    (*count)++;

    return clients;
}


//Checks if a client ID already exists.
int client_id_exists(const Client* clients, int count, int id)
{
    return find_client_index_by_id(clients, count, id) != -1;
}




//find client index , recorrido lineal, no modifica datos.

/*Searches for a client by ID using linear search.
@param clients Clients array
@param count   Number of clients
@param id      ID to search
@return Index of the client if found, -1 otherwise.*/
 

int find_client_index_by_id (const Client* clients, int count, int id)
{
	int i;
	for  (i = 0; i < count; i++)
	if (clients [i].id ==id){
		return i;
	}
	
	
	
	return -1;
}

// edit client
/* Updates the name of an existing client.
@param clients Clients array
@param count   Number of clients
@param id      ID of the client to edit
@param new_name New name for the client
@return 1 if the client was updated, 0 if not found.*/

int edit_client(Client* clients, int count , int id , const char*  new_name)
{
 int index = find_client_index_by_id (clients, count, id);
 
 if(index == -1 ){
 return 0; //cliente no encontrado

 }	
	
 
 
 
 
 strncpy (clients [ index].name, new_name , sizeof(clients[index]. name) -1);
 clients[index].name [sizeof(clients[index].name) -1] = '\0';
 
 return 1; // exito
 	
}


// ================= REMOVE ================= //
/* Removes a client identified by ID.
Shifts remaining clients and resizes memory.
@param clients Clients array
@param count   Pointer to number of clients
@param id      ID of the client to remove
@return Updated clients array (may be NULL if empty)*/

Client* remove_client_by_id(Client* clients, int* count, int id)
{
    int i, index = -1;

    for (i = 0; i < *count; i++) {
        if (clients[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) return clients;

    for (i = index; i < *count - 1; i++) {
        clients[i] = clients[i + 1];
    }

    (*count)--;

    if (*count == 0) {
        free(clients);
        return NULL;
    }

    Client* temp = realloc(clients, sizeof(Client) * (*count));
    return temp ? temp : clients;
}

//================= PRINT ================= //
/* Prints all registered clients to stdout.
@param clients Clients array
@param count   Number of clients */

void print_all_clients(Client* clients, int count)
{
    int i;
	for ( i = 0; i < count; i++) {
        print_client(&clients[i]);
        printf("-----------------\n");
    }
}

// ================= FREE ================= //
/* Frees all memory allocated for clients.
@param clients Clients array
@param count   Number of clients */




void free_all_clients(Client* clients, int count){
	
    if (clients != NULL) {
    free(clients);
	
	}


}

