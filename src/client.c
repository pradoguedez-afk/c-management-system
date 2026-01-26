#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"


Client* create_client(int id, const char*name){
	Client* client = (Client*) malloc(sizeof(Client));
	
	if (client == NULL){
		return NULL;
		
	}
	
	client->id = id;
	strncpy (client ->name, name , sizeof (client->name)-1);
	client  -> name [sizeof(client->name)-1] = '\0';
	
	return client;
	
	
}

void print_client (const  Client* client){
	if (client == NULL)return ;
	
	
	printf ("Client  ID: %d\n", client->id);
	printf ("Client Name: %s\n", client->name);
	
}

void free_client (Client* client ){
	free(client);
	
}
