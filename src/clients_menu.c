#include <stdio.h>
#include "client.h"
#include "client_manager.h"
#include "clients_menu.h"
#include "menu.h"

void clients_menu(Client** clients, int* count)
{
    int option;
    int id;
    char name[100];

    do {
        show_clients_menu();
        option = read_option_in_range(0, 4);

        switch (option) {

            case 1:
                printf("Client ID: ");
                scanf("%d", &id);

                printf("Client name: ");
                scanf(" %99[^\n]", name);

                if (client_id_exists(*clients, *count, id)) {
                printf("\nError: Client ID already exists.\n");
                break;
                }

				
				*clients = add_client(*clients, count, id, name);
                printf("\nClient added successfully!\n");
                break;

            case 2:
                printf("Client ID to remove: ");
                scanf("%d", &id);

                *clients = remove_client_by_id(*clients, count, id);
                printf("\nClient removed (if it existed).\n");
                break;

            case 3:
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

                if (edit_client(*clients, *count, id, name)) {
                printf("\nClient updated successfully!\n");
                } else {
                printf("\nClient not found.\n");
                }
                break;

                case 0:
                printf("\nReturning to main menu...\n");
                break;

                default:
                printf("\nInvalid option.\n");
        }

    } while (option != 0);
}

