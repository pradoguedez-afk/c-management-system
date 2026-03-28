/*
 * ventas.c - Sales module implementation.
 * Integrates with clients (validation) and products (validation + stock).
 * Realloc safety: we never lose the previous pointer until the new one is valid.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ventas.h"

/* ---------------------------------------------------------------------------
 * Internal helpers (resolve ids to names for printing).
 * --------------------------------------------------------------------------- */
static const char* get_client_name(int client_id, const Client* clients, int client_count)
{
    int i;
    if (clients == NULL || client_count <= 0) return NULL;
    for (i = 0; i < client_count; i++) {
        if (clients[i].id == client_id)
            return clients[i].name;
    }
    return NULL;
}

static const char* get_product_name(int product_id, const Product* products, int product_count)
{
    int i;
    if (products == NULL || product_count <= 0) return NULL;
    for (i = 0; i < product_count; i++) {
        if (products[i].id == product_id)
            return products[i].name;
    }
    return NULL;
}

/* ---------------------------------------------------------------------------
 * Validation (Step 2).
 * --------------------------------------------------------------------------- */

int validate_sale_date(int year, int month, int day)
{
    int feb_days;
    static const int days_per_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    feb_days = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
    if (month == 2) {
        if (day > feb_days) return 0;
    } else if (day > days_per_month[month - 1]) {
        return 0;
    }
    return 1;
}

int validate_client_id(int client_id, const Client* clients, int client_count)
{
    int i;
    if (clients == NULL || client_count <= 0) return 0;
    for (i = 0; i < client_count; i++) {
        if (clients[i].id == client_id) return 1;
    }
    return 0;
}

int validate_sale_item(int product_id, int quantity, float unit_price,
                       const Product* products, int product_count)
{
    int idx;
    if (products == NULL || product_count <= 0) return 0;
    if (quantity <= 0) return 0;
    if (unit_price < 0.0f) return 0;
    idx = find_product_index_by_id(products, product_count, product_id);
    return idx >= 0 ? 1 : 0;
}

/* ---------------------------------------------------------------------------
 * Model: create, add item, print, free.
 * --------------------------------------------------------------------------- */

Sale* create_sale(int id, int client_id, int year, int month, int day)
{
    Sale* sale = (Sale*)malloc(sizeof(Sale));
    if (sale == NULL) return NULL;

    sale->id = id;
    sale->client_id = client_id;
    sale->date.year = year;
    sale->date.month = month;
    sale->date.day = day;
    sale->item_count = 0;
    sale->items = NULL;
    sale->total = 0.0f;

    return sale;
}

int sale_add_item(Sale* sale, int product_id, int quantity, float unit_price)
{
    SaleItem* temp;
    if (sale == NULL || quantity <= 0) return 0;

    temp = (SaleItem*)realloc(sale->items, sizeof(SaleItem) * (size_t)(sale->item_count + 1));
    if (temp == NULL) return 0;

    sale->items = temp;
    sale->items[sale->item_count].product_id = product_id;
    sale->items[sale->item_count].quantity = quantity;
    sale->items[sale->item_count].unit_price = unit_price;
    sale->total += (float)quantity * unit_price;
    sale->item_count++;

    return 1;
}

void print_sale(const Sale* sale, const Client* clients, int client_count,
                const Product* products, int product_count)
{
    int i;
    const char* name;
    float subtotal;

    if (sale == NULL) return;

    printf("--- Venta ID: %d ---\n", sale->id);
    printf("Cliente ID: %d", sale->client_id);
    name = get_client_name(sale->client_id, clients, client_count);
    if (name != NULL)
        printf(" (%s)", name);
    printf("\n");
    printf("Fecha: %04d-%02d-%02d\n", sale->date.year, sale->date.month, sale->date.day);

    if (sale->item_count == 0) {
        printf("(Sin líneas)\n");
    } else {
        for (i = 0; i < sale->item_count; i++) {
            name = get_product_name(sale->items[i].product_id, products, product_count);
            subtotal = (float)sale->items[i].quantity * sale->items[i].unit_price;
            printf("  Producto ID %d", sale->items[i].product_id);
            if (name != NULL) printf(" (%s)", name);
            printf(": %d x %.2f = %.2f\n",
                   sale->items[i].quantity, sale->items[i].unit_price, subtotal);
        }
    }
    printf("TOTAL: %.2f\n", sale->total);
}

void free_sale(Sale* sale)
{
    if (sale == NULL) return;
    free(sale->items);
    sale->items = NULL;
    free(sale);
}

/* ---------------------------------------------------------------------------
 * Validated add item: validation + stock deduction (Step 3 inventory).
 * --------------------------------------------------------------------------- */

VentasResult sale_add_item_validated(Sale* sale, int product_id, int quantity, float unit_price,
                                     Product* products, int product_count)
{
    int stock;
    if (sale == NULL) return VENTAS_ERR_MEMORY;
    if (!validate_sale_item(product_id, quantity, unit_price, products, product_count))
        return VENTAS_ERR_INVALID_PRODUCT;
    if (quantity <= 0) return VENTAS_ERR_INVALID_QUANTITY;
    if (unit_price < 0.0f) return VENTAS_ERR_INVALID_PRICE;

    stock = get_product_stock(products, product_count, product_id);
    if (stock < 0) return VENTAS_ERR_INVALID_PRODUCT;
    if (stock < quantity) return VENTAS_ERR_INSUFFICIENT_STOCK;

    if (!subtract_product_stock(products, product_count, product_id, quantity))
        return VENTAS_ERR_INSUFFICIENT_STOCK;
    if (!sale_add_item(sale, product_id, quantity, unit_price)) {
        add_product_stock(products, product_count, product_id, quantity);
        return VENTAS_ERR_MEMORY;
    }
    int index = find_product_index_by_id(products, product_count, product_id);

    if (index != -1) {
        Product p = products[index];
        save_sale_to_file(p.id, p.name, quantity, p.price);
    }

       
    return VENTAS_OK;
}

/* ---------------------------------------------------------------------------
 * Manager: deep copy, add, remove (with stock restore), find, list, free.
 * --------------------------------------------------------------------------- */

/* Deep copy of a sale (struct + items array). On item copy failure, sale is left with 0 items. */
static Sale copy_sale(const Sale* src)
{
    Sale dest;
    size_t n;

    dest.id = src->id;
    dest.client_id = src->client_id;
    dest.date = src->date;
    dest.item_count = src->item_count;
    dest.total = src->total;
    dest.items = NULL;

    if (src->item_count > 0 && src->items != NULL) {
        n = (size_t)src->item_count * sizeof(SaleItem);
        dest.items = (SaleItem*)malloc(n);
        if (dest.items != NULL) {
            memcpy(dest.items, src->items, n);
        } else {
            dest.item_count = 0;
            dest.total = 0.0f;
        }
    }
    return dest;
}

Sale* add_sale(Sale* sales, int* count, const Sale* new_sale)
{
    Sale* temp;
    if (new_sale == NULL || count == NULL) return sales;

    temp = (Sale*)realloc(sales, sizeof(Sale) * (size_t)(*count + 1));
    if (temp == NULL) return sales;

    sales = temp;
    sales[*count] = copy_sale(new_sale);
    (*count)++;
    return sales;
}

Sale* remove_sale_by_id(Sale* sales, int* count, int id,
                        Product* products, int product_count)
{
    int i, index = find_sale_index_by_id(sales, *count, id);
    Sale* temp;

    if (index == -1) return sales;

    /* Restore product stock for each item (Step 3 integration). */
    if (products != NULL && product_count > 0 && sales[index].items != NULL) {
        for (i = 0; i < sales[index].item_count; i++) {
            add_product_stock(products, product_count,
                              sales[index].items[i].product_id,
                              sales[index].items[i].quantity);
        }
    }

    free(sales[index].items);
    sales[index].items = NULL;

    for (i = index; i < *count - 1; i++)
        sales[i] = sales[i + 1];
    (*count)--;

    if (*count == 0) {
        free(sales);
        return NULL;
    }

    temp = (Sale*)realloc(sales, sizeof(Sale) * (size_t)(*count));
    return temp != NULL ? temp : sales;
}

int find_sale_index_by_id(const Sale* sales, int count, int id)
{
    int i;
    for (i = 0; i < count; i++) {
        if (sales[i].id == id) return i;
    }
    return -1;
}

int sale_id_exists(const Sale* sales, int count, int id)
{
    return find_sale_index_by_id(sales, count, id) != -1;
}

void print_all_sales(const Sale* sales, int count, const Client* clients, int client_count,
                     const Product* products, int product_count)
{
    int i;
    for (i = 0; i < count; i++) {
        print_sale(&sales[i], clients, client_count, products, product_count);
        printf("-----------------\n");
    }
}

void free_all_sales(Sale* sales, int count)
{
    int i;
    if (sales == NULL) return;
    for (i = 0; i < count; i++) {
        free(sales[i].items);
        sales[i].items = NULL;
    }
    free(sales);
}

/* ---------------------------------------------------------------------------
 * Sales interface: register, list, search, delete, reports (Step 4).
 * --------------------------------------------------------------------------- */

VentasResult register_sale(Sale** sales, int* sale_count, int new_sale_id, int client_id,
                            int year, int month, int day, const int* product_ids,
                            const int* quantities, int item_count,
                            const Client* clients, int client_count,
                            Product* products, int product_count)
{
    Sale* sale = NULL;
    int i;
    int idx;
    float price;
    VentasResult res;

    if (sales == NULL || sale_count == NULL || product_ids == NULL || quantities == NULL)
        return VENTAS_ERR_MEMORY;

    if (!validate_client_id(client_id, clients, client_count))
        return VENTAS_ERR_INVALID_CLIENT;
    if (!validate_sale_date(year, month, day))
        return VENTAS_ERR_INVALID_DATE;
    if (sale_id_exists(*sales, *sale_count, new_sale_id))
        return VENTAS_ERR_DUPLICATE_SALE_ID;

    /* Pre-validate all items and stock so we don't partially apply. */
    for (i = 0; i < item_count; i++) {
        if (!validate_sale_item(product_ids[i], quantities[i], 0.0f, products, product_count))
            return VENTAS_ERR_INVALID_PRODUCT;
        if (quantities[i] <= 0) return VENTAS_ERR_INVALID_QUANTITY;
        idx = find_product_index_by_id(products, product_count, product_ids[i]);
        if (idx < 0) return VENTAS_ERR_INVALID_PRODUCT;
        if (products[idx].stock < quantities[i]) return VENTAS_ERR_INSUFFICIENT_STOCK;
    }

    sale = create_sale(new_sale_id, client_id, year, month, day);
    if (sale == NULL) return VENTAS_ERR_MEMORY;

    for (i = 0; i < item_count; i++) {
        idx = find_product_index_by_id(products, product_count, product_ids[i]);
        price = products[idx].price;
        res = sale_add_item_validated(sale, product_ids[i], quantities[i], price,
                                      products, product_count);
        if (res != VENTAS_OK) {
            /* Rollback: restore stock for items already added. */
            while (--i >= 0) {
                add_product_stock(products, product_count, product_ids[i], quantities[i]);
            }
            free_sale(sale);
            return res;
        }
    }

    {
        int prev_count = *sale_count;
        Sale* new_arr = add_sale(*sales, sale_count, sale);
        if (*sale_count != prev_count + 1) {
            /* realloc failed; count unchanged. Restore stock and free sale. */
            for (i = 0; i < sale->item_count; i++) {
                add_product_stock(products, product_count,
                                  sale->items[i].product_id, sale->items[i].quantity);
            }
            free_sale(sale);
            return VENTAS_ERR_MEMORY;
        }
        *sales = new_arr;
    }
    free_sale(sale);
    return VENTAS_OK;
}

void list_all_sales(const Sale* sales, int count, const Client* clients, int client_count,
                    const Product* products, int product_count)
{
    print_all_sales(sales, count, clients, client_count, products, product_count);
}

void search_sale_by_id(const Sale* sales, int count, int id,
                       const Client* clients, int client_count,
                       const Product* products, int product_count)
{
    int idx = find_sale_index_by_id(sales, count, id);
    if (idx >= 0)
        print_sale(&sales[idx], clients, client_count, products, product_count);
    else
        printf("Venta con ID %d no encontrada.\n", id);
}

int delete_sale_by_id(Sale** sales, int* sale_count, int id,
                      Product* products, int product_count)
{
    int idx;
    Sale* new_arr;
    if (sales == NULL || sale_count == NULL) return 0;
    idx = find_sale_index_by_id(*sales, *sale_count, id);
    if (idx < 0) return 0;
    new_arr = remove_sale_by_id(*sales, sale_count, id, products, product_count);
    *sales = new_arr;
    return 1;
}

void sales_by_client(const Sale* sales, int count, int client_id,
                     const Client* clients, int client_count,
                     const Product* products, int product_count)
{
    int i, any = 0;
    for (i = 0; i < count; i++) {
        if (sales[i].client_id == client_id) {
            print_sale(&sales[i], clients, client_count, products, product_count);
            printf("-----------------\n");
            any = 1;
        }
    }
    if (!any)
        printf("No hay ventas para el cliente ID %d.\n", client_id);
}

void sales_by_date(const Sale* sales, int count, int year, int month, int day,
                   const Client* clients, int client_count,
                   const Product* products, int product_count)
{
    int i, any = 0;
    for (i = 0; i < count; i++) {
        if (sales[i].date.year == year && sales[i].date.month == month && sales[i].date.day == day) {
            print_sale(&sales[i], clients, client_count, products, product_count);
            printf("-----------------\n");
            any = 1;
        }
    }
    if (!any)
        printf("No hay ventas en la fecha %04d-%02d-%02d.\n", year, month, day);
}

/*
 * TODO (Step 5 - future features):
 * - save_sales_to_file() / load_sales_from_file(): persist sales array to binary file
 *   (format: count, then for each sale: Sale header + item_count + SaleItem array).
 * - sales_report(): summary by period (total revenue, number of sales).
 * - top_selling_products(): aggregate quantities from all sales, sort by quantity sold.
 * - generate_invoice(sale_id): format one sale as a simple text or PDF invoice.
 */

 void save_sale_to_file(int id, const char* name, int quantity, float price)
 {
     FILE* f = fopen("sales.csv", "a");
 
     if (f != NULL) {
         fprintf(f, "%d,%s,%d,%.2f,%.2f\n",
             id,
             name,
             quantity,
             price,
             price * quantity
         );
         fclose(f);
     }
 }
 
 void init_sales_file()
{
    FILE* f = fopen("sales.csv", "r");

    if (f == NULL) {
        f = fopen("sales.csv", "w");
        if (f != NULL) {
            fprintf(f, "id,product,quantity,price,total\n");
        }
    }

    if (f != NULL) fclose(f);
}

void load_sales_from_file()
{
    FILE* f = fopen("sales.csv", "r");

    if (f == NULL) {
        printf("No sales file found.\n");
        return;
    }

    int id, quantity;
    float price, total;
    char name[50];

    printf("\n=== SALES LOADED ===\n");

    while (fscanf(f, "%d,%49[^,],%d,%f,%f\n",
                  &id, name, &quantity, &price, &total) == 5)
    {
        printf("ID: %d | Product: %s | Qty: %d | Price: %.2f | Total: %.2f\n",
               id, name, quantity, price, total);
    }

    fclose(f);
}

