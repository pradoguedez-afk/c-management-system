#ifndef PRODUCT_H
#define PRODUCT_H

#define PRODUCT_NAME_LEN 50

typedef struct {
    int id;
    char name[PRODUCT_NAME_LEN];
    float price;
} Product;

/* Basic product utilities */
Product* create_product(int id, const char* name, float price);
void print_product(const Product* product);
void free_product(Product* product);

#endif

