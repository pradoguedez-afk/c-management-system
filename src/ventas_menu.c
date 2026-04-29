#include <stdio.h>
#include <stdlib.h>

#include "ventas_menu.h"
#include "ventas.h"


/**
 * @brief Imprime en consola el menú visual de opciones de ventas.
 * * Muestra las opciones disponibles para el usuario, incluyendo registro,
 * listado, búsqueda y eliminación de ventas.
 */
static void show_sales_menu(void)
{
    printf("\n=== SALES MENU ===\n");
    printf("1. Register sale\n");
    printf("2. List sales\n");
    printf("3. Search sale by ID\n");
    printf("4. Delete sale by ID\n");
    printf("5. Sales by client (optional)\n");
    printf("6. Sales by date (optional)\n");
    printf("0. Back\n");
    printf("Select an option: ");
}

/**
 * @brief Limpia el búfer de entrada estándar (stdin).
 * * Consume los caracteres restantes en el búfer hasta encontrar un salto de línea
 * o el final del archivo (EOF) para evitar errores en lecturas posteriores.
 */
static void clear_input_buffer(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}


/**
 * @brief Realiza una lectura segura de un número entero desde consola.
 * * @param prompt Mensaje de texto a mostrar al usuario.
 * @param value Puntero a la variable donde se almacenará el entero leído.
 * @return int Retorna 1 si la lectura fue exitosa, 0 en caso contrario.
 */
static int read_int_safe(const char* prompt, int* value)
{
    int result;

    if (value == NULL) return 0;

    printf("%s", prompt);
    result = scanf("%d", value);

    if (result != 1) {
        clear_input_buffer();
        return 0;
    }

    clear_input_buffer();
    return 1;
}


/**
 * @brief Realiza una lectura segura de una fecha (Año, Mes, Día).
 * * @param prompt Mensaje informativo para el usuario.
 * @param year Puntero para almacenar el año.
 * @param month Puntero para almacenar el mes.
 * @param day Puntero para almacenar el día.
 * @return int Retorna 1 si se leyeron los 3 valores correctamente, 0 en caso contrario.
 */
static int read_date_safe(const char* prompt, int* year, int* month, int* day)
{
    int result;

    if (year == NULL || month == NULL || day == NULL) return 0;

    printf("%s", prompt);
    result = scanf("%d %d %d", year, month, day);

    if (result != 3) {
        clear_input_buffer();
        return 0;
    }

    clear_input_buffer();
    return 1;
}


/**
 * @brief Muestra mensajes de retroalimentación basados en el resultado de una operación de ventas.
 * * @param res Enumeración de tipo VentasResult que indica el estado de la operación.
 */
static void print_ventas_result(VentasResult res)
{
    switch (res) {
        case VENTAS_OK:
            printf("\nSale registered successfully.\n");
            break;
        case VENTAS_ERR_INVALID_CLIENT:
            printf("\nError: invalid client.\n");
            break;
        case VENTAS_ERR_INVALID_PRODUCT:
            printf("\nError: invalid product.\n");
            break;
        case VENTAS_ERR_INVALID_QUANTITY:
            printf("\nError: invalid quantity.\n");
            break;
        case VENTAS_ERR_INVALID_PRICE:
            printf("\nError: invalid price.\n");
            break;
        case VENTAS_ERR_INVALID_DATE:
            printf("\nError: invalid date.\n");
            break;
        case VENTAS_ERR_INSUFFICIENT_STOCK:
            printf("\nError: insufficient stock.\n");
            break;
        case VENTAS_ERR_DUPLICATE_SALE_ID:
            printf("\nError: sale ID already exists.\n");
            break;
        case VENTAS_ERR_MEMORY:
        default:
            printf("\nError: memory / internal error.\n");
            break;
    }
}

/**
 * @brief Función principal del submenú de ventas.
 * * Gestiona el bucle de interacción para todas las operaciones de venta.
 * Se encarga de la validación inicial de datos, gestión de memoria dinámica 
 * para productos en una venta y la llamada a las funciones de lógica de negocio.
 * * @param sales Puntero al arreglo dinámico de punteros de Sale.
 * @param sale_count Puntero al contador total de ventas registradas.
 * @param clients Arreglo constante de clientes existentes.
 * @param client_count Cantidad de clientes registrados.
 * @param products Arreglo de productos disponibles.
 * @param product_count Cantidad de productos en inventario.
 */
void ventas_menu(Sale** sales, int* sale_count,
                 const Client* clients, int client_count,
                 Product* products, int product_count)
{
    int option;

    if (sales == NULL || sale_count == NULL) {
        printf("\nInternal error: sales pointers not provided.\n");
        return;
    }

    do {
        show_sales_menu();
        option = read_option_in_range(0, 6);

        switch (option) {
            case 1: {
                int sale_id, client_id;
                int year, month, day;
                int item_count, i;
                int* product_ids = NULL;
                int* quantities = NULL;
                VentasResult res;
                int input_ok = 1;

                if (!read_int_safe("Sale ID: ", &sale_id)) {
                    printf("\nError: invalid Sale ID.\n");
                    break;
                }

                if (!read_int_safe("Client ID: ", &client_id)) {
                    printf("\nError: invalid Client ID.\n");
                    break;
                }

                if (!read_date_safe("Date (YYYY MM DD): ", &year, &month, &day)) {
                    printf("\nError: invalid date input.\n");
                    break;
                }

                if (!read_int_safe("Number of items: ", &item_count)) {
                    printf("\nError: invalid item count.\n");
                    break;
                }

                if (item_count <= 0) {
                    printf("\nError: item count must be greater than 0.\n");
                    break;
                }

                product_ids = (int*)malloc(sizeof(int) * (size_t)item_count);
                quantities = (int*)malloc(sizeof(int) * (size_t)item_count);

                if (product_ids == NULL || quantities == NULL) {
                    free(product_ids);
                    free(quantities);
                    printf("\nError: memory allocation failed.\n");
                    break;
                }

                for (i = 0; i < item_count; i++) {
                    if (!read_int_safe("Item Product ID: ", &product_ids[i])) {
                        printf("\nError: invalid product ID for item %d.\n", i + 1);
                        input_ok = 0;
                        break;
                    }

                    if (!read_int_safe("Item Quantity: ", &quantities[i])) {
                        printf("\nError: invalid quantity for item %d.\n", i + 1);
                        input_ok = 0;
                        break;
                    }
                }

                if (!input_ok) {
                    free(product_ids);
                    free(quantities);
                    break;
                }

                res = register_sale(sales, sale_count, sale_id, client_id,
                                    year, month, day,
                                    product_ids, quantities, item_count,
                                    clients, client_count,
                                    products, product_count);

                print_ventas_result(res);

                free(product_ids);
                free(quantities);
                break;
            }

            case 2:
            load_sales_from_file();
                break;

            case 3: {
                int id;

                if (!read_int_safe("Sale ID to search: ", &id)) {
                    printf("\nError: invalid Sale ID.\n");
                    break;
                }

                search_sale_by_id(*sales, *sale_count, id,
                                  clients, client_count,
                                  products, product_count);
                break;
            }

            case 4: {
                int id;
                int deleted;

                if (!read_int_safe("Sale ID to delete: ", &id)) {
                    printf("\nError: invalid Sale ID.\n");
                    break;
                }

                deleted = delete_sale_by_id(sales, sale_count, id,
                                            products, product_count);

                if (deleted)
                    printf("\nSale deleted successfully.\n");
                else
                    printf("\nSale not found.\n");
                break;
            }

            case 5: {
                int cid;

                if (!read_int_safe("Client ID: ", &cid)) {
                    printf("\nError: invalid Client ID.\n");
                    break;
                }

                sales_by_client(*sales, *sale_count, cid,
                                clients, client_count,
                                products, product_count);
                break;
            }

            case 6: {
                int y, m, d;

                if (!read_date_safe("Date (YYYY MM DD): ", &y, &m, &d)) {
                    printf("\nError: invalid date input.\n");
                    break;
                }

                sales_by_date(*sales, *sale_count, y, m, d,
                              clients, client_count,
                              products, product_count);
                break;
            }

            case 0:
                printf("\nReturning to main menu...\n");
                break;

            default:
                printf("\nInvalid option.\n");
                break;
        }

    } while (option != 0);
}

