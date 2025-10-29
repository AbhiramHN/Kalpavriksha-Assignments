#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define NAME_LENGTH 50

typedef struct
{
    int productId;
    char productName[NAME_LENGTH];
    float price;
    int quantity;
} Product;

Product *createInitialProducts(int *count)
{
    printf("Enter initial number of products: ");
    scanf("%d", count);

    Product *products = (Product *)calloc(*count, sizeof(Product));
    if (products == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    for (int index = 0; index < *count; index++)
    {
        printf("\nEnter details for product %d:\n", index + 1);
        printf("Product ID: ");
        scanf("%d", &products[index].productId);
        printf("Product Name: ");
        scanf("%s", products[index].productName);
        printf("Product Price: ");
        scanf("%f", &products[index].price);
        printf("Product Quantity: ");
        scanf("%d", &products[index].quantity);
    }
    return products;
}

void displayMenu()
{
    printf("\n----------- INVENTORY MENU -----------\n");
    printf("1. Add New Product\n");
    printf("2. View All Products\n");
    printf("3. Update Quantity\n");
    printf("4. Search Product by ID\n");
    printf("5. Search Product by Name\n");
    printf("6. Search Product by Price Range\n");
    printf("7. Delete Product\n");
    printf("8. Exit\n");
    printf("Enter your choice: ");
}

void viewAllProducts(const Product *products, int count)
{
    if (count == 0)
    {
        printf("\nNo products available.\n");
        return;
    }
    printf("\n------------- PRODUCT LIST -----------\n");
    for (int index = 0; index < count; index++)
    {
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
               products[index].productId,
               products[index].productName,
               products[index].price,
               products[index].quantity);
    }
}

void addNewProduct(Product **products, int *count)
{
    *products = realloc(*products, (*count + 1) * sizeof(Product));
    if (*products == NULL)
    {
        printf("Memory reallocation failed.\n");
        exit(1);
    }
    printf("\nEnter new product details:\n");
    printf("Product ID: ");
    scanf("%d", &(*products)[*count].productId);
    printf("Product Name: ");
    scanf("%s", (*products)[*count].productName);
    printf("Product Price: ");
    scanf("%f", &(*products)[*count].price);
    printf("Product Quantity: ");
    scanf("%d", &(*products)[*count].quantity);

    (*count)++;
    printf("Product added successfully\n");
}

void updateQuantity(Product *products, int count)
{
    int id;
    printf("Enter Product ID to update quantity: ");
    scanf("%d", &id);
    for (int index = 0; index < count; index++)
    {
        if (products[index].productId == id)
        {
            printf("Enter new Quantity: ");
            scanf("%d", &products[index].quantity);
            printf("Quantity updated successfully\n");
            return;
        }
    }
    printf("Product not found.\n");
}

void searchProductById(const Product *products, int count)
{
    int id;
    printf("Enter Product ID to search: ");
    scanf("%d", &id);
    for (int index = 0; index < count; index++)
    {
        if (products[index].productId == id)
        {
            printf("Product Found: Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[index].productId,
                   products[index].productName,
                   products[index].price,
                   products[index].quantity);
            return;
        }
    }
    printf("Product not found.\n");
}

void searchProductByName(const Product *products, int count)
{
    char searchName[NAME_LENGTH];
    printf("Enter product name to search: ");
    scanf("%s", searchName);

    int found = 0;
    printf("\nProducts Found:\n");
    for (int index = 0; index < count; index++)
    {
        if (strstr(products[index].productName, searchName) != NULL)
        {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[index].productId,
                   products[index].productName,
                   products[index].price,
                   products[index].quantity);
            found = 1;
        }
    }
    if (!found)
    {
        printf("No product found with that name.\n");
    }
}

void searchProductByPriceRange(const Product *products, int count)
{
    float minPrice, maxPrice;
    printf("Enter minimum price: ");
    scanf("%f", &minPrice);
    printf("Enter maximum price: ");
    scanf("%f", &maxPrice);

    int found = 0;
    printf("\nProducts in price range:\n");

    for (int index = 0; index < count; index++)
    {
        if (products[index].price >= minPrice && products[index].price <= maxPrice)
        {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[index].productId,
                   products[index].productName,
                   products[index].price,
                   products[index].quantity);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No products found in this range.\n");
    }
}

void deleteProductById(Product **products, int *count)
{
    int id, found = 0;
    printf("Enter Product ID to delete: ");
    scanf("%d", &id);

    for (int index = 0; index < *count; index++)
    {
        if ((*products)[index].productId == id)
        {
            for (int shiftIndex = index; shiftIndex < *count - 1; shiftIndex++)
            {
                (*products)[shiftIndex] = (*products)[shiftIndex + 1];
            }
            (*count)--;

            *products = realloc(*products, (*count) * sizeof(Product));
            printf("Product deleted successfully\n");
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("Product not found.\n");
    }
}

void freeMemory(Product *products)
{
    free(products);
    printf("\nMemory released successfully....\n");
}

void handleUserChoice(Product **products, int *count, int choice)
{
    switch (choice)
    {
    case 1:
        addNewProduct(products, count);
        break;
    case 2:
        viewAllProducts(*products, *count);
        break;
    case 3:
        updateQuantity(*products, *count);
        break;
    case 4:
        searchProductById(*products, *count);
        break;
    case 5:
        searchProductByName(*products, *count);
        break;
    case 6:
        searchProductByPriceRange(*products, *count);
        break;
    case 7:
        deleteProductById(products, count);
        break;
    case 8:
        freeMemory(*products);
        exit(0);
    default:
        printf("Invalid choice.\n");
    }
}

int main()
{
    int productCount;
    Product *products = createInitialProducts(&productCount);

    while (1)
    {
        int choice;
        displayMenu();
        scanf("%d", &choice);
        handleUserChoice(&products, &productCount, choice);
    }
    return 0;
}
