#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_TABLES 20
#define MAX_ITEMS 10
#define MAX_NAME_LENGTH 40
#define FILENAME_LENGTH 40

typedef struct {
    char name[MAX_NAME_LENGTH];
    int quantity;
} Item;

typedef struct {
    char name[MAX_NAME_LENGTH];
    Item items[MAX_ITEMS];
    int itemCount;
} Table;

Table tables[MAX_TABLES];
int tableCount = 0;

void loadTableData(int index) {
    FILE *file;
    char filename[FILENAME_LENGTH];

    snprintf(filename, FILENAME_LENGTH, "%s.txt", tables[index].name);

    file = fopen(filename, "r");
    if (file != NULL) {
        fscanf(file, "%d", &tables[index].itemCount);

        for (int i = 0; i < tables[index].itemCount; i++) {
            fscanf(file, " %30[^\n]", tables[index].items[i].name);
        }

        fclose(file);
    }
}

void saveTableData(int index) {
    FILE *file;
    char filename[FILENAME_LENGTH];

    snprintf(filename, FILENAME_LENGTH, "%s.txt", tables[index].name);

    file = fopen(filename, "w");
    if (file != NULL) {
        fprintf(file, "%d\n", tables[index].itemCount);

        for (int i = 0; i < tables[index].itemCount; i++) {
            fprintf(file, "%s\n", tables[index].items[i].name);
        }

        fclose(file);
    }
}

void loadAllTables() {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(".")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt") != NULL) {
                char tableName[MAX_NAME_LENGTH];
                sscanf(ent->d_name, "%30[^.]", tableName);

                if (tableCount < MAX_TABLES) {
                    strncpy(tables[tableCount].name, tableName, MAX_NAME_LENGTH);
                    loadTableData(tableCount);
                    tableCount++;
                } else {
                    printf("Maximum number of tables reached!\n");
                    break;
                }
            }
        }
        closedir(dir);
    } else {
        perror("Error opening directory");
    }
}

void addTable() {
    if (tableCount < MAX_TABLES) {
        printf("Enter the name of the new table: ");
        fflush(stdin);
        fgets(tables[tableCount].name, MAX_NAME_LENGTH, stdin);
        tables[tableCount].name[strcspn(tables[tableCount].name, "\n")] = '\0';
        tables[tableCount].itemCount = 0;

        saveTableData(tableCount);

        tableCount++;
        printf("Table added successfully!\n");
    } else {
        printf("Maximum number of tables reached!\n");
    }
}

void changeTableName() {
    if (tableCount > 0) {
        int tableIndex;
        printf("Enter the index of the table you want to rename (0 to %d): ", tableCount - 1);
        scanf("%d", &tableIndex);

        if (tableIndex >= 0 && tableIndex < tableCount) {
            printf("Enter the new name for the table: ");
            fflush(stdin);
            fgets(tables[tableIndex].name, MAX_NAME_LENGTH, stdin);
            tables[tableIndex].name[strcspn(tables[tableIndex].name, "\n")] = '\0';

            saveTableData(tableIndex);

            printf("Table name changed successfully!\n");
        } else {
            printf("Invalid table index!\n");
        }
    } else {
        printf("No tables available!\n");
    }
}

void deleteTable() {
    if (tableCount > 0) {
        int tableIndex;
        printf("Enter the index of the table you want to delete (0 to %d): ", tableCount - 1);
        scanf("%d", &tableIndex);

        if (tableIndex >= 0 && tableIndex < tableCount) {
            char filename[FILENAME_LENGTH];
            snprintf(filename, FILENAME_LENGTH, "%s.txt", tables[tableIndex].name);
            remove(filename);

            for (int i = tableIndex; i < tableCount - 1; i++) {
                tables[i] = tables[i + 1];
            }
            tableCount--;
            printf("Table deleted successfully!\n");
        } else {
            printf("Invalid table index!\n");
        }
    } else {
        printf("No tables available!\n");
    }
}

void addItem() {
    if (tableCount > 0) {
        int tableIndex;
        printf("Enter the index of the table to add an item (0 to %d): ", tableCount - 1);
        scanf("%d", &tableIndex);

        if (tableIndex >= 0 && tableIndex < tableCount) {
            if (tables[tableIndex].itemCount < MAX_ITEMS) {
                printf("Enter the name of the item: ");
                fflush(stdin);
                fgets(tables[tableIndex].items[tables[tableIndex].itemCount].name, MAX_NAME_LENGTH, stdin);
                tables[tableIndex].items[tables[tableIndex].itemCount].name[strcspn(tables[tableIndex].items[tables[tableIndex].itemCount].name, "\n")] = '\0';

                tables[tableIndex].itemCount++;

                saveTableData(tableIndex);

                printf("Item added successfully!\n");
            } else {
                printf("Maximum number of items reached for the table!\n");
            }
        } else {
            printf("Invalid table index!\n");
        }
    } else {
        printf("No tables available!\n");
    }
}


void deleteItem() {
    if (tableCount > 0) {
        int tableIndex;
        printf("Enter the index of the table from which you want to delete an item (0 to %d): ", tableCount - 1);
        scanf("%d", &tableIndex);

        if (tableIndex >= 0 && tableIndex < tableCount) {
            if (tables[tableIndex].itemCount > 0) {
                int itemIndex;
                printf("Enter the index of the item you want to delete (0 to %d): ", tables[tableIndex].itemCount - 1);
                scanf("%d", &itemIndex);

                if (itemIndex >= 0 && itemIndex < tables[tableIndex].itemCount) {
                    for (int i = itemIndex; i < tables[tableIndex].itemCount - 1; i++) {
                        tables[tableIndex].items[i] = tables[tableIndex].items[i + 1];
                    }
                    tables[tableIndex].itemCount--;

                    saveTableData(tableIndex);

                    printf("Item deleted successfully!\n");
                } else {
                    printf("Invalid item index!\n");
                }
            } else {
                printf("No items in the table!\n");
            }
        } else {
            printf("Invalid table index!\n");
        }
    } else {
        printf("No tables available!\n");
    }
}

void changeItemName() {
    if (tableCount > 0) {
        int tableIndex;
        printf("Enter the index of the table from which you want to change an item's name (0 to %d): ", tableCount - 1);
        scanf("%d", &tableIndex);

        if (tableIndex >= 0 && tableIndex < tableCount) {
            if (tables[tableIndex].itemCount > 0) {
                int itemIndex;
                printf("Enter the index of the item whose name you want to change (0 to %d): ", tables[tableIndex].itemCount - 1);
                scanf("%d", &itemIndex);

                if (itemIndex >= 0 && itemIndex < tables[tableIndex].itemCount) {
                    printf("Enter the new name for the item: ");
                    fflush(stdin);
                    fgets(tables[tableIndex].items[itemIndex].name, MAX_NAME_LENGTH, stdin);
                    tables[tableIndex].items[itemIndex].name[strcspn(tables[tableIndex].items[itemIndex].name, "\n")] = '\0';

                    saveTableData(tableIndex);

                    printf("Item name changed successfully!\n");
                } else {
                    printf("Invalid item index!\n");
                }
            } else {
                printf("No items in the table!\n");
            }
        } else {
            printf("Invalid table index!\n");
        }
    } else {
        printf("No tables available!\n");
    }
}

void displayTables() {
    if (tableCount > 0) {
        for (int i = 0; i < tableCount; i++) {
            printf("%s\n", tables[i].name);

            if (tables[i].itemCount > 0) {
                for (int j = 0; j < tables[i].itemCount; j++) {
                    printf("%s\n", tables[i].items[j].name);                    
                }
                printf("\n");
            } else {
                printf("   No items in this table.\n");
            }
        }
    } else {
        printf("No tables available!\n");
    }
}

int main() {
    int choice;

    loadAllTables();

    do {
        printf("\nDatabase Application Menu:\n");
        printf("1. Add Table\n");
        printf("2. Change Table Name\n");
        printf("3. Delete Table\n");
        printf("4. Add Item to Table\n");
        printf("5. Delete Item from Table\n");
        printf("6. Change Item Name in Table\n");
        printf("7. Display Tables\n");
        printf("0. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addTable();
                break;
            case 2:
                changeTableName();
                break;
            case 3:
                deleteTable();
                break;
            case 4:
                addItem();
                break;
            case 5:
                deleteItem();
                break;
            case 6:
                changeItemName();
                break;
            case 7:
                displayTables();
                break;
            case 0:
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    return 0;
}