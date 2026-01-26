#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "product_manager.h"

/* ================= ADD ================= */

Product* add_product(Product* products, int* count, int id, const char* name, float price)
{
    Product* temp = realloc(products, sizeof(Product) * (*count + 1));
    if (!temp) return products;

    products = temp;
    products[*count].id = id;
    products[*count].price = price;
    strcpy(products[*count].name, name);

    (*count)++;
    return products;
}

/* ================= FIND ================= */

int find_product_index_by_id(const Product* products, int count, int id)
{
    int i;
    for (i = 0; i < count; i++) {
        if (products[i].id == id) {
            return i;
        }
    }
    return -1;
}

/* ================= EDIT ================= */

int edit_product(Product* products, int count, int id, const char* new_name, float new_price)
{
    int index = find_product_index_by_id(products, count, id);

    if (index == -1) {
        return 0; // product not found
    }

    strncpy(products[index].name, new_name, sizeof(products[index].name) - 1);
    products[index].name[sizeof(products[index].name) - 1] = '\0';
    products[index].price = new_price;

    return 1; // success
}

/* ================= REMOVE ================= */

Product* remove_product_by_id(Product* products, int* count, int id)
{
    int i, index = -1;

    for (i = 0; i < *count; i++) {
        if (products[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) return products;

    for (i = index; i < *count - 1; i++) {
        products[i] = products[i + 1];
    }

    (*count)--;

    if (*count == 0) {
        free(products);
        return NULL;
    }

    Product* temp = realloc(products, sizeof(Product) * (*count));
    return temp ? temp : products;
}

/* ================= PRINT ================= */

void print_all_products(Product* products, int count)
{
    int i;
    for (i = 0; i < count; i++) {
        print_product(&products[i]);
        printf("-----------------\n");
    }
}

/* ================= FREE ================= */

void free_all_products(Product* products, int count)
{
    if (products != NULL) {
        free(products);
    }
}

