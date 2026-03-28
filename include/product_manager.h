#ifndef PRODUCT_MANAGER_H
#define PRODUCT_MANAGER_H

#include "product.h"

/* Product management */

Product* add_product(Product* products, int* count, int id, const char* name, float price, int stock);

Product* remove_product_by_id(Product* products, int* count, int id);

void print_all_products(Product* products, int count);

void free_all_products(Product* products, int count);

int find_product_index_by_id(const Product* products, int count, int id);

int edit_product(Product* products, int count, int id, const char* new_name, float new_price, int new_stock);
/* Stock (inventory): for sales integration */
int get_product_stock(const Product* products, int count, int product_id);
int subtract_product_stock(Product* products, int count, int product_id, int quantity);
void add_product_stock(Product* products, int count, int product_id, int quantity);

#endif

