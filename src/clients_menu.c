/**
 * @file clients_menu.c
 * @brief Implementation of the interactive menu for client management.
 */

#include <stdio.h>
#include "client.h"
#include "client_manager.h"
#include "clients_menu.h"
#include "menu.h"

/**
 * @brief Handles the client management menu loop.
 * * Provides options to add, remove, list, and edit clients. It updates 
 * the client array and its count based on user input.
 * * @param clients Double pointer to the clients array (to handle reallocation).
 * @param count   Pointer to the integer tracking the number of clients.
 */
void clients_menu(Client** clients, int* count)
{
    int option;
    int id;
    char name[100];

    do {
/* Display menu options and capture user choice */
        show_clients_menu();
        option = read_option_in_range(0, 4);

        switch (option) {

            case 1: /* ADD CLIENT */
                printf("Client ID: ");
                scanf("%d", &id);

                printf("Client name: ");
                scanf(" %99[^\n]", name);
/* Validate if ID is unique before adding */
                if (client_id_exists(*clients, *count, id)) {
                printf("\nError: Client ID already exists.\n");
                break;
                }

/* Add client and update the pointer in case of realloc */				
				*clients = add_client(*clients, count, id, name);
                printf("\nClient added successfully!\n");
                break;

            case 2: /* REMOVE CLIENT */
                printf("Client ID to remove: ");
                scanf("%d", &id);

/* Remove client and update the pointer in case of memory shrink */
                *clients = remove_client_by_id(*clients, count, id);
                printf("\nClient removed (if it existed).\n");
                break;

            case 3: /* LIST CLIENTS */
                /* DEBUG temporal  */
                printf("DEBUG MENU: count = %d\n", *count);

                if (*count == 0) {
                    printf("\nNo clients registered.\n");
                } else {
                    print_all_clients(*clients, *count);
                }
                break;
                
                case 4:  // EDIT CLIENT
                  if (*count == 0) {
                  printf("\nNo clients to edit.\n");
                  break;
                  }

                  printf("Client ID to edit: ");
                  scanf("%d", &id);

                  printf("New client name: ");
                  scanf(" %99[^\n]", name);
/* Attempt to update existing client information */
                if (edit_client(*clients, *count, id, name)) {
                printf("\nClient updated successfully!\n");
                } else {
                printf("\nClient not found.\n");
                }
                break;

                case 0: /* EXIT */
                printf("\nReturning to main menu...\n");
                break;

                default:
                printf("\nInvalid option.\n");
        }

    } while (option != 0);
}

