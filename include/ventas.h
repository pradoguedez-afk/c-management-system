/**
 * @file ventas.h
 * @brief Sales management module and inventory synchronization.
 * * This module handles the registration of sales, date validation, 
 * and ensures that product stock is correctly updated when sales 
 * are created or deleted.
 */
#ifndef VENTAS_H
#define VENTAS_H

#include "client.h"
#include "product.h"
#include "product_manager.h"
/**
 * @enum VentasResult
 * @brief Result codes for sale operations and validations.
 */


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



/**
 * @struct SaleDate
 * @brief Simplified date structure for sale records.
 */
typedef struct {
    int year;
    int month;
    int day;
} SaleDate;


/**
 * @struct SaleItem
 * @brief Represents a single line item within a sale.
 */
typedef struct {
    int product_id;
    int quantity;
    float unit_price;
} SaleItem;


/**
 * @struct Sale
 * @brief Main structure for a sale transaction.
 * @note The 'items' member is a dynamically allocated array.
 */
typedef struct {
    int id;
    int client_id;
    SaleDate date;
    int item_count;
    SaleItem* items;
    float total;
} Sale;


// Sale Model Fuctions//

/**
 * @brief Initializes a new Sale structure in memory.
 * @return Pointer to the new sale, or NULL if allocation fails.
 */
Sale* create_sale(int id, int client_id, int year, int month, int day);

/**
 * @brief Adds a SaleItem to a specific sale and updates the total price.
 * @return 1 on success, 0 on failure.
 */
int sale_add_item(Sale* sale, int product_id, int quantity, float unit_price);

/**
 * @brief Prints a detailed receipt of a sale.
 * @param clients/products Optional. If provided, shows names instead of just IDs.
 */
void print_sale(const Sale* sale, const Client* clients, int client_count,
                const Product* products, int product_count);

/**
 * @brief Prints a detailed receipt of a sale.
 * @param clients/products Optional. If provided, shows names instead of just IDs.
 */
void free_sale(Sale* sale);


//Validation Fuctions

/** @brief Checks if the date is calendar-valid. */
int validate_sale_date(int year, int month, int day);

/** @brief Checks if a client exists in the database. */
int validate_client_id(int client_id, const Client* clients, int client_count);

/** @brief Checks item parameters (quantity > 0, price >= 0, product exists). */
int validate_sale_item(int product_id, int quantity, float unit_price,
                       const Product* products, int product_count);


 // Global Sales Manager (CRUD)
 

/**
 * @brief Appends a deep copy of a sale to the global sales array.
 * @param sales Pointer to the current sales array.
 * @param count Pointer to the total count of sales.
 * @param new_sale The sale to be added.
 * @return Updated pointer to the sales array.
 */
Sale* add_sale(Sale* sales, int* count, const Sale* new_sale);


/**
 * @brief Removes a sale and optionally restores product stock.
 * @param products If not NULL, the stock of the items in this sale will be added back.
 */
Sale* remove_sale_by_id(Sale* sales, int* count, int id,
                        Product* products, int product_count);


/** @brief Finds a sale index by ID. Returns -1 if not found. */
int find_sale_index_by_id(const Sale* sales, int count, int id);

/** @brief Checks if a sale ID is already taken. */
int sale_id_exists(const Sale* sales, int count, int id);

/** @brief Lists all registered sales in the system. */
void print_all_sales(const Sale* sales, int count, const Client* clients, int client_count,
                     const Product* products, int product_count);

/** @brief Cleans up all memory used by the sales manager. */
void free_all_sales(Sale* sales, int count);


// Inventory Integration

/**
 * @brief Validates an item and subtracts its quantity from the product stock.
 * @return VENTAS_OK or specific error code if stock is insufficient.
 */
VentasResult sale_add_item_validated(Sale* sale, int product_id, int quantity, float unit_price,
                                     Product* products, int product_count);


//High-Level Interface

/**
 * @brief Orchestrates the complete registration of a sale.
 * This function handles validation, stock subtraction, and storage in the sales array.
 * @param sales Pointer to the pointer of the sales array (may be reallocated).
 * @return VENTAS_OK on success, or error code explaining the failure.
 */
VentasResult register_sale(Sale** sales, int* sale_count, int new_sale_id, int client_id,
                            int year, int month, int day, const int* product_ids,
                            const int* quantities, int item_count,
                            const Client* clients, int client_count,
                            Product* products, int product_count);

/** @brief Wrapper to list all sales. */
void list_all_sales(const Sale* sales, int count, const Client* clients, int client_count,
                    const Product* products, int product_count);

/** @brief Searches for a sale and displays it, or shows an error if not found. */
void search_sale_by_id(const Sale* sales, int count, int id,
                       const Client* clients, int client_count,
                       const Product* products, int product_count);

/** @brief Deletes a sale and restores stock. Returns 1 on success, 0 if not found. */
int delete_sale_by_id(Sale** sales, int* sale_count, int id,
                      Product* products, int product_count);

/** @brief Filters and prints sales associated with a specific client. */
void sales_by_client(const Sale* sales, int count, int client_id,
                     const Client* clients, int client_count,
                     const Product* products, int product_count);

/** @brief Filters and prints sales associated with a specific client. */
void sales_by_date(const Sale* sales, int count, int year, int month, int day,
                   const Client* clients, int client_count,
                   const Product* products, int product_count);

//File Persistence Binary

/** @brief Saves a single transaction record to the history file. */
 void save_sale_to_file(int id, const char* name, int quantity, float price);

 /** @brief Saves a single transaction record to the history file. */
 void init_sales_file();

 /** @brief Loads the sales history from the permanent storage. */
 void load_sales_from_file();

#endif /* VENTAS_H */
