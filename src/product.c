#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "product.h"

void print_product(const Product* product)
{
    if (product == NULL) return;

    printf("Product ID: %d\n", product->id);
    printf("Name      : %s\n", product->name);
    printf("Price     : %.2f\n", product->price);
    printf("Stock     : %d\n", product->stock);
}
