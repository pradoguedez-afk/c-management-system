#ifndef PRODUCTS_FILE_H
#define PRODUCTS_FILE_H

#include "product.h"

Product* load_products(const char* filename, int* count);
void save_products(const char* filename, Product* products, int count);

#endif

