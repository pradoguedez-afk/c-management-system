/*
 * ventas.h - Sales module (ventas)
 * Depends on: client.h, product.h, product_manager.h (for stock operations).
 * Header guards and types are self-contained; include after client and product.
 */
#ifndef VENTAS_H
#define VENTAS_H

#include "client.h"
#include "product.h"
#include "product_manager.h"

/* ---------------------------------------------------------------------------
 * Result codes for validation and register_sale (caller can show messages).
 * --------------------------------------------------------------------------- */
typedef enum {
    VENTAS_OK = 0,
    VENTAS_ERR_INVALID_CLIENT,
    VENTAS_ERR_INVALID_PRODUCT,
    VENTAS_ERR_INVALID_QUANTITY,
    VENTAS_ERR_INVALID_PRICE,
    VENTAS_ERR_INVALID_DATE,
    VENTAS_ERR_INSUFFICIENT_STOCK,
    VENTAS_ERR_DUPLICATE_SALE_ID,
    VENTAS_ERR_MEMORY
} VentasResult;

/* ---------------------------------------------------------------------------
 * Types (SaleDate must be before SaleItem and Sale).
 * --------------------------------------------------------------------------- */

/* Date of a sale (avoids dependency on time.h; easy to serialize). */
typedef struct {
    int year;
    int month;
    int day;
} SaleDate;

/* Line item: product, quantity, and unit price at time of sale. */
typedef struct {
    int product_id;
    int quantity;
    float unit_price;
} SaleItem;

/* Sale: client, date, dynamic list of items, and total. */
typedef struct {
    int id;
    int client_id;
    SaleDate date;
    int item_count;
    SaleItem* items;
    float total;
} Sale;

/* ---------------------------------------------------------------------------
 * Model: create, add item, print, free (single sale).
 * --------------------------------------------------------------------------- */

/* Creates an empty sale (no items). Returns NULL on allocation failure. */
Sale* create_sale(int id, int client_id, int year, int month, int day);

/* Adds one line to the sale; updates total. Returns 1 on success, 0 on failure. */
int sale_add_item(Sale* sale, int product_id, int quantity, float unit_price);

/* Prints one sale; pass clients/products (may be NULL) to show names. */
void print_sale(const Sale* sale, const Client* clients, int client_count,
                const Product* products, int product_count);

/* Frees the sale and its items array. Safe to call with NULL. */
void free_sale(Sale* sale);

/* ---------------------------------------------------------------------------
 * Validation (use before create_sale / sale_add_item / add_sale).
 * --------------------------------------------------------------------------- */

/* Returns 1 if date is valid (reasonable year/month/day), 0 otherwise. */
int validate_sale_date(int year, int month, int day);

/* Returns 1 if client_id exists in the client list, 0 otherwise. */
int validate_client_id(int client_id, const Client* clients, int client_count);

/* Returns 1 if product exists, quantity > 0, unit_price >= 0; 0 otherwise. */
int validate_sale_item(int product_id, int quantity, float unit_price,
                       const Product* products, int product_count);

/* ---------------------------------------------------------------------------
 * Manager: CRUD and listing (sales array).
 * --------------------------------------------------------------------------- */

/* Appends a deep copy of new_sale. Returns possibly new pointer (realloc). */
Sale* add_sale(Sale* sales, int* count, const Sale* new_sale);

/* Removes sale by id; if products != NULL restores stock. Returns possibly new pointer. */
Sale* remove_sale_by_id(Sale* sales, int* count, int id,
                        Product* products, int product_count);

/* Returns index of sale with given id, or -1. */
int find_sale_index_by_id(const Sale* sales, int count, int id);

/* Returns 1 if a sale with this id already exists, 0 otherwise. */
int sale_id_exists(const Sale* sales, int count, int id);

/* Prints all sales; pass clients/products for names (may be NULL). */
void print_all_sales(const Sale* sales, int count, const Client* clients, int client_count,
                     const Product* products, int product_count);

/* Frees all sales and the sales array. Safe if sales is NULL. */
void free_all_sales(Sale* sales, int count);

/* ---------------------------------------------------------------------------
 * Validated add item: checks product, quantity, price, and stock; subtracts stock.
 * Use this when registering a sale so inventory stays consistent.
 * --------------------------------------------------------------------------- */

/* Validates and adds item; on success subtracts quantity from product stock.
 * Returns VENTAS_ERR_* or VENTAS_OK. products/count required for stock. */
VentasResult sale_add_item_validated(Sale* sale, int product_id, int quantity, float unit_price,
                                     Product* products, int product_count);

/* ---------------------------------------------------------------------------
 * Sales interface (for main/menu): register, list, search, delete, reports.
 * --------------------------------------------------------------------------- */

/* Registers a new sale: validates client, date, and each item (and stock);
 * creates sale, adds items with stock deduction, appends to sales array.
 * On failure returns error code and does not modify sales or stock.
 * new_sale_id must be unique (not in sales). */
VentasResult register_sale(Sale** sales, int* sale_count, int new_sale_id, int client_id,
                            int year, int month, int day, const int* product_ids,
                            const int* quantities, int item_count,
                            const Client* clients, int client_count,
                            Product* products, int product_count);

/* Lists all sales (wrapper around print_all_sales). */
void list_all_sales(const Sale* sales, int count, const Client* clients, int client_count,
                    const Product* products, int product_count);

/* Finds sale by id and prints it; prints a message if not found. */
void search_sale_by_id(const Sale* sales, int count, int id,
                       const Client* clients, int client_count,
                       const Product* products, int product_count);

/* Deletes sale by id and restores product stock. Returns 1 if deleted, 0 if not found. */
int delete_sale_by_id(Sale** sales, int* sale_count, int id,
                      Product* products, int product_count);

/* Optional: prints all sales for a given client_id. */
void sales_by_client(const Sale* sales, int count, int client_id,
                     const Client* clients, int client_count,
                     const Product* products, int product_count);

/* Optional: prints all sales on a given date. */
void sales_by_date(const Sale* sales, int count, int year, int month, int day,
                   const Client* clients, int client_count,
                   const Product* products, int product_count);

 void save_sale_to_file(int id, const char* name, int quantity, float price);

 void init_sales_file();

 void load_sales_from_file();

#endif /* VENTAS_H */
