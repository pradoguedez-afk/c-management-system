#include <stdio.h>
#include <stdlib.h>
#include "clients_file.h"


Client* load_clients(const char* filename, int* count)
{
    FILE* file = fopen(filename, "rb");
    if (!file) {
        *count = 0;
        return NULL;
    }

    fread(count, sizeof(int), 1, file);

    Client* clients = malloc(sizeof(Client) * (*count));
    if (!clients) {
        fclose(file);
        *count = 0;
        return NULL;
    }

    fread(clients, sizeof(Client), *count, file);
    fclose(file);

    return clients;
}

void save_clients(const char* filename, Client* clients, int count)
{
    FILE* file = fopen(filename, "wb");
    if (!file) return;

    fwrite(&count, sizeof(int), 1, file);
    fwrite(clients, sizeof(Client), count, file);

    fclose(file);
}



