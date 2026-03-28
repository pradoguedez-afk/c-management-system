#ifndef PRODUCT_H
#define PRODUCT_H

#define PRODUCT_NAME_LEN 50

typedef struct {
    int id;
    char name[PRODUCT_NAME_LEN];
    float price;
    int stock;
} Product;

void print_product(const Product* product);

#endif

