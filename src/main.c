#include <stdio.h>
#include "client.h"
#include "client_manager.h"
#include "clients_menu.h"
#include "clients_file.h"
#include "menu.h"
#include "product.h"
#include "product_manager.h"
#include "products_menu.h"
#include "products_file.h"
#include "ventas.h"
#include "ventas_menu.h"

int main(void)
{
	Product* products = NULL;
    int product_count = 0;

    Sale* sales = NULL;
    int sale_count = 0;
	
	int option = -1;
    Client* clients = NULL;
    int count = 0;

    /* Cargar clientes y productos desde archivo*/
    clients = load_clients("clients.dat", &count);
    products = load_products("products.dat", &product_count);

    do {
        show_main_menu();
        option = read_option_in_range(0, 3);

        switch (option) {

            case 1:
                clients_menu(&clients, &count);
                break;

            case 2:
                products_menu(&products, &product_count);
                break;

            case 3:
                ventas_menu(&sales, &sale_count, clients, count, products, product_count);
                break;

            case 0:
                printf("\nExiting system...\n");
                break;

            default:
                printf("\nInvalid option.\n");
        }

    } while (option != 0);

    /* Guardar y liberar */
    save_clients("clients.dat", clients, count);
    free_all_clients(clients, count);

    /* Guardar y liberar productos */
    save_products("products.dat", products, product_count);
    free_all_products(products, product_count);

    /* Liberar ventas (no persistence yet) */
    free_all_sales(sales, sale_count);
    
    // guardar ventas
    init_sales_file();
	
	
	return 0;
}


