#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a product
struct Product {
    int id;
    char name[50];
    double price;
    int quantity;
};

// File to store the inventory data
const char* FILE_NAME = "inventory.txt";

// Function to load the inventory from a file
int loadInventory(struct Product inventory[], int* productCount) {
    FILE* file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        return 0;  // File doesn't exist, no products loaded
    }

    *productCount = 0;
    while (fread(&inventory[*productCount], sizeof(struct Product), 1, file) == 1) {
        (*productCount)++;
    }

    fclose(file);
    return 1;  // Products loaded successfully
}

// Function to save the inventory to a file
void saveInventory(struct Product inventory[], int productCount) {
    FILE* file = fopen(FILE_NAME, "w");
    if (file == NULL) {
        printf("Error saving inventory data.\n");
        return;
    }

    for (int i = 0; i < productCount; i++) {
        fwrite(&inventory[i], sizeof(struct Product), 1, file);
    }

    fclose(file);
    printf("Inventory data saved.\n");
}

// Function to find a product by ID
int findProduct(struct Product inventory[], int productCount, int productId) {
    for (int i = 0; i < productCount; i++) {
        if (inventory[i].id == productId) {
            return i;  // Product found, return its index
        }
    }
    return -1;  // Product not found
}

// Function to add a product to the inventory
void addProduct(struct Product inventory[], int* productCount) {
    if (*productCount < 100) {
        struct Product product;
        printf("Enter product ID: ");
        scanf("%d", &product.id);
        if (findProduct(inventory, *productCount, product.id) != -1) {
            printf("Product with the same ID already exists.\n");
            return;
        }
        printf("Enter product name: ");
        scanf("%s", product.name);
        printf("Enter product price: ");
        scanf("%lf", &product.price);
        printf("Enter product quantity: ");
        scanf("%d", &product.quantity);
        inventory[*productCount] = product;
        (*productCount)++;
        saveInventory(inventory, *productCount);
        printf("Product added to inventory.\n");
    } else {
        printf("Inventory is full. Cannot add more products.\n");
    }
}

// Function to remove a product from the inventory
void removeProduct(struct Product inventory[], int* productCount) {
    int productId;
    printf("Enter product ID to remove: ");
    scanf("%d", &productId);

    int productIndex = findProduct(inventory, *productCount, productId);
    if (productIndex != -1) {
        for (int i = productIndex; i < *productCount - 1; i++) {
            inventory[i] = inventory[i + 1];
        }
        (*productCount)--;
        saveInventory(inventory, *productCount);
        printf("Product removed from inventory.\n");
    } else {
        printf("Product not found in inventory.\n");
    }
}

// Function to edit a product's details
void editProduct(struct Product inventory[], int productCount) {
    int productId;
    printf("Enter product ID to edit: ");
    scanf("%d", &productId);

    int productIndex = findProduct(inventory, productCount, productId);
    if (productIndex != -1) {
        struct Product* product = &inventory[productIndex];
        printf("Enter new product name (or press Enter to keep '%s'): ", product->name);
        char newName[50];
        getchar();  // Clear the newline character
        fgets(newName, sizeof(newName), stdin);
        if (strlen(newName) > 1) {
            newName[strcspn(newName, "\n")] = 0;  // Remove the newline character
            strcpy(product->name, newName);
        }

        printf("Enter new product price (or press Enter to keep %.2lf): ", product->price);
        char priceInput[50];
        fgets(priceInput, sizeof(priceInput), stdin);
        if (strlen(priceInput) > 1) {
            product->price = atof(priceInput);
        }

        printf("Enter new product quantity (or press Enter to keep %d): ", product->quantity);
        char quantityInput[50];
        fgets(quantityInput, sizeof(quantityInput), stdin);
        if (strlen(quantityInput) > 1) {
            product->quantity = atoi(quantityInput);
        }

        saveInventory(inventory, productCount);
        printf("Product details updated.\n");
    } else {
        printf("Product not found in inventory.\n");
    }
}

// Function to list all products in the inventory
void listProducts(struct Product inventory[], int productCount) {
    printf("Inventory List:\n");
    for (int i = 0; i < productCount; i++) {
        printf("ID: %d, Name: %s, Price: %.2lf, Quantity: %d\n", inventory[i].id, inventory[i].name, inventory[i].price, inventory[i].quantity);
    }
}

int main() {
    struct Product inventory[100];
    int productCount = 0;

    if (loadInventory(inventory, &productCount)) {
        printf("Inventory data loaded.\n");
    } else {
        printf("No inventory data found.\n");
    }

    while (1) {
        printf("\nInventory Management System\n");
        printf("1. Add Product\n");
        printf("2. Remove Product\n");
        printf("3. Edit Product\n");
        printf("4. List Products\n");
        printf("5. Quit\n");
        printf("Enter your choice: ");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addProduct(inventory, &productCount);
                break;
            case 2:
                removeProduct(inventory, &productCount);
                break;
            case 3:
                editProduct(inventory, productCount);
                break;
            case 4:
                listProducts(inventory, productCount);
                break;
            case 5:
                saveInventory(inventory, productCount);
                printf("Inventory data saved. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
