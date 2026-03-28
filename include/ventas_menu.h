/* @file ventas_menu.h
 * @brief User interface menu for sales (ventas) operations.
 *
 * This module handles user interaction related to sales.
 * It displays menu options and delegates actions to the ventas module.
 * It does not implement business logic or file operations.
 */
#ifndef VENTAS_MENU_H
#define VENTAS_MENU_H

#include "ventas.h"
#include "client.h"
#include "product.h"

/* Displays the sales menu and handles user choices.
 * @param sales       Pointer to the sales array (may be reallocated)
 * @param sale_count  Pointer to number of sales
 * @param clients     Clients list (read-only)
 * @param client_count Number of clients
 * @param products    Products list (stock is updated on register/delete)
 * @param product_count Number of products
 */
void ventas_menu(Sale** sales, int* sale_count,
                 const Client* clients, int client_count,
                 Product* products, int product_count);

int read_option(void);
int read_option_in_range(int min, int max);

#endif /* VENTAS_MENU_H */

