#include <stdio.h>

void save_clients(Client* clients, int count) {
    FILE* file = fopen("clientes.dat", "wb");

    if (file == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    fwrite(&count, sizeof(int), 1, file);          // guardar cantidad
    fwrite(clients, sizeof(Client), count, file);  // guardar clientes

    fclose(file);
}

