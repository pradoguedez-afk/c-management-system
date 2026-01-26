#include <stdio.h>
#include <stdlib.h>
#include "products_file.h"

Product* load_products(const char* filename, int* count)
{
    FILE* file = fopen(filename, "rb");
    if (!file) {
        *count = 0;
        return NULL;
    }

    fread(count, sizeof(int), 1, file);

    Product* products = malloc(sizeof(Product) * (*count));
    if (!products) {
        fclose(file);
        *count = 0;
        return NULL;
    }

    fread(products, sizeof(Product), *count, file);
    fclose(file);

    return products;
}

void save_products(const char* filename, Product* products, int count)
{
    FILE* file = fopen(filename, "wb");
    if (!file) return;

    fwrite(&count, sizeof(int), 1, file);
    fwrite(products, sizeof(Product), count, file);
    fclose(file);
}

