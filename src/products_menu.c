#include <stdio.h>
#include "product.h"
#include "product_manager.h"
#include "products_menu.h"
#include "menu.h"

void products_menu(Product** products, int* count)
{
    int option;
    int id;
    char name[100];
    float price;
    int stock; 

    do {
        printf("\n=== PRODUCTS MENU ===\n");
        printf("1. Add product\n");
        printf("2. Remove product\n");
        printf("3. List products\n");
        printf("4. Edit product\n");
        printf("0. Back\n");

        option = read_option();

        switch (option) {

            case 1:
                printf("Product ID: ");
                scanf("%d", &id);

                printf("Product name: ");
                scanf(" %99[^\n]", name);

                printf("Product price: ");
                scanf("%f", &price);

                printf("Stock: ");
                scanf("%d", &stock);

                *products = add_product(*products, count, id, name, price, stock);
                printf("\nProduct added successfully!\n");
                break;

            case 2:
                printf("Product ID to remove: ");
                scanf("%d", &id);

                *products = remove_product_by_id(*products, count, id);
                printf("\nProduct removed (if it existed).\n");
                break;

            case 3:
                if (*count == 0) {
                    printf("\nNo products registered.\n");
                } else {
                    print_all_products(*products, *count);
                }
                break;

            case 4:
                if (*count == 0) {
                    printf("\nNo products to edit.\n");
                    break;
                }

                printf("Product ID to edit: ");
                scanf("%d", &id);

                printf("New product name: ");
                scanf(" %99[^\n]", name);

                printf("New product price: ");
                scanf("%f", &price);

                printf("New stock: ");
                scanf("%d", &stock);

                if (edit_product(*products, *count, id, name, price, stock)) {
                    printf("\nProduct updated successfully!\n");
                } else {
                    printf("\nProduct not found.\n");
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

