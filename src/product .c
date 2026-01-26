#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "product.h"

Product* create_product(int id, const char* name, float price)
{
    Product* product = malloc(sizeof(Product));
    if (!product) return NULL;

    product->id = id;
    product->price = price;

    strncpy(product->name, name, PRODUCT_NAME_LEN - 1);
    product->name[PRODUCT_NAME_LEN - 1] = '\0';

    return product;
}

void print_product(const Product* product)
{
    if (product == NULL) return;

    printf("Product ID: %d\n", product->id);
    printf("Name      : %s\n", product->name);
    printf("Price     : %.2f\n", product->price);
}

void free_product(Product* product)
{
    free(product);
}

