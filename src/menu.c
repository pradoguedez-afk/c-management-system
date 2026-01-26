#include <stdio.h>
#include "menu.h"
#include <stdlib.h> 
#include <string.h>

void show_main_menu(void) {
    printf("\n=== MANAGEMENT SYSTEM ===\n");
    printf("1. Manage clients\n");
    printf("2. Manage products\n");
    printf("0. Exit\n");
    printf("Select an option: ");
}

void show_clients_menu(void) {
    printf("\n=== CLIENTS MENU ===\n");
    printf("1. Add client\n");
    printf("2. Remove client\n");
    printf("3. List clients\n");
    printf("4. Edit client\n");
    printf("0. Back\n");
    printf("Select an option: ");
}

int read_option(void) {
    char buffer[32];
    int option;

    while (1) {

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error. Try again.\n");
            continue;
        }

        // Remove newline
        buffer[strcspn(buffer, "\n")] = '\0';

        char *endptr;
        option = (int)strtol(buffer, &endptr, 10);

        if (*endptr != '\0') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        return option;
    }
}

int read_option_in_range(int min, int max)
{
    int option;

    while (1) {
        option = read_option();

        if (option < min || option > max) {
            printf("Invalid option. Please select between %d and %d.\n", min, max);
            continue;
        }

        return option;
    }
}


