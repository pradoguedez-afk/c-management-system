#ifndef CLIENT_H
#define CLIENT_H

typedef struct {
	int id;
	char name [50];
	
	} Client ;
	
	
Client* create_client (int id, const char* name );
void print_client ( const Client*  client );
void free_client (Client* client);


#endif 
