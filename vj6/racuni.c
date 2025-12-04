#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "racuni.h"

Account* loadAllAccounts(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("Greska: ne mogu otvoriti datoteku %s\n", path);
        return NULL;
    }

    Account* head = NULL; 
    char line[256];

    while (fgets(line, sizeof(line), file)) {

        line[strcspn(line, "\n")] = '\0';

        Account* acc = loadSingleAccount(line);
        head = insertAccountSorted(head, acc);
    }

    fclose(file);
    return head;
}


Account* loadSingleAccount(const char* filename) {

    char fullPath[256];
    sprintf(fullPath, "racuni/%s", filename);

    FILE* file = fopen(fullPath, "r");
    if (!file) {
        printf("Greska: ne mogu otvoriti racun %s\n", fullPath);
        return NULL;
    }

    Account* acc = malloc(sizeof(Account));
    if (!acc) {
        printf("Greska: problem s alokacijom memorije za racun.\n");
        fclose(file);
        return NULL;
    }

    acc->items = NULL;
    acc->next = NULL;

    if (!fgets(acc->date, sizeof(acc->date), file)) {
        printf("Greska: ne mogu procitati datum iz %s\n", fullPath);
        free(acc);
        fclose(file);
        return NULL;
    }

    acc->date[strcspn(acc->date, "\n")] = '\0';
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {

        line[strcspn(line, "\n")] = '\0';

        char name[100];
        int qty;
        double price;

        if (sscanf(line, "%[^,], %d, %lf", name, &qty, &price) == 3) {
            acc->items = insertItemSorted(acc->items, createItem(name, qty, price));
        }
    }

    fclose(file);
    return acc;
}

Account* insertAccountSorted(Account* head, Account* newAcc) {
    if (newAcc == NULL)
        return head;

    if (head == NULL || strcmp(newAcc->date, head->date) < 0) {
        newAcc->next = head;
        return newAcc;
    }

    Account* current = head;

    while (current->next != NULL &&
           strcmp(newAcc->date, current->next->date) > 0) {
        current = current->next;
    }

    newAcc->next = current->next;
    current->next = newAcc;

    return head;
}

int printAccounts(Account* head) {
    if (head == NULL) {
        printf("Nema dostupnih racuna za prikaz.\n");
        return -1;
    }

    Account* currentAcc = head;

    while (currentAcc != NULL) {
        printf("\nDatum: %s\n", currentAcc->date);
        printf("------------------------\n");

        Item* currentItem = currentAcc->items;

        while (currentItem != NULL) {
            printf("  %-20s kolicina: %d   cijena: %.2lf EUR\n",
                   currentItem->name,
                   currentItem->quantity,
                   currentItem->price);

            currentItem = currentItem->next;
        }

        currentAcc = currentAcc->next;
        printf("\n");
    }

    return 0;  
}

double totalSpentAll(Account* head, const char* itemName) {
    double total = 0.0;
    Account* acc = head;

    while (acc != NULL) {
        Item* item = acc->items;

        while (item != NULL) {
            if (strcmp(item->name, itemName) == 0) {
                total += item->quantity * item->price;
            }
            item = item->next;
        }

        acc = acc->next;
    }

    return total;
}

double calculateSpent(Account* head, const char* itemName,
                      const char* fromDate, const char* toDate) {

    double total = 0.0;
    Account* acc = head;

    while (acc != NULL) {

        if (strcmp(acc->date, fromDate) >= 0 &&
            strcmp(acc->date, toDate) <= 0) {

            Item* item = acc->items;

            while (item != NULL) {
                if (strcmp(item->name, itemName) == 0) {
                    total += item->quantity * item->price;
                }
                item = item->next;
            }
        }

        acc = acc->next;
    }

    return total;
}

int quantityInPeriod(Account* head, const char* itemName,
                     const char* fromDate, const char* toDate) 
{
    int totalQty = 0;
    Account* acc = head;

    while (acc != NULL) {
        
        if (strcmp(acc->date, fromDate) >= 0 &&
            strcmp(acc->date, toDate) <= 0)
        {
            Item* item = acc->items;

            while (item != NULL) {
                if (strcmp(item->name, itemName) == 0) {
                    totalQty += item->quantity;  
                }
                item = item->next;
            }
        }

        acc = acc->next;
    }

    return totalQty;
}

int quantityInAll(Account* head, const char* itemName) {
    int totalQty = 0;
    Account* acc = head;

    while (acc != NULL) {
        Item* item = acc->items;

        while (item != NULL) {
            if (strcmp(item->name, itemName) == 0) {
                totalQty += item->quantity;
            }
            item = item->next;
        }

        acc = acc->next;
    }

    return totalQty;
}
int mostBoughtItem(Account* head, char* resultName, int* resultQty) {
    if (head == NULL) return -1;

    char seen[500][100];  
    int seenCount = 0;

    Account* acc = head;

    while (acc != NULL) {
        Item* item = acc->items;

        while (item != NULL) {

            int exists = 0;
            for (int i = 0; i < seenCount; i++) {
                if (strcmp(seen[i], item->name) == 0) {
                    exists = 1;
                    break;
                }
            }

            if (!exists) {
                strcpy(seen[seenCount], item->name);
                seenCount++;
            }

            item = item->next;
        }

        acc = acc->next;
    }

    int maxQty = -1;
    char maxName[100] = "";

    for (int i = 0; i < seenCount; i++) {
        int qty = quantityInAll(head, seen[i]);

        if (qty > maxQty) {
            maxQty = qty;
            strcpy(maxName, seen[i]);
        }
    }

    strcpy(resultName, maxName);
    *resultQty = maxQty;

    return 0;
}
int mostSpentItem(Account* head, char* resultName, double* resultMoney) {
    if (head == NULL) return -1;

    char seen[500][100]; 
    int seenCount = 0;

    Account* acc = head;

    while (acc != NULL) {
        Item* item = acc->items;

        while (item != NULL) {

            int exists = 0;
            for (int i = 0; i < seenCount; i++) {
                if (strcmp(seen[i], item->name) == 0) {
                    exists = 1;
                    break;
                }
            }

            if (!exists) {
                strcpy(seen[seenCount], item->name);
                seenCount++;
            }

            item = item->next;
        }

        acc = acc->next;
    }

    double maxSpent = -1.0;
    char maxName[100] = "";

    for (int i = 0; i < seenCount; i++) {
        double spent = totalSpentAll(head, seen[i]);

        if (spent > maxSpent) {
            maxSpent = spent;
            strcpy(maxName, seen[i]);
        }
    }

    strcpy(resultName, maxName);
    *resultMoney = maxSpent;

    return 0;
}
int listAllItems(Account* head) {
    if (head == NULL) {
        printf("Nema racuna u sustavu.\n");
        return -1;
    }

    char seen[500][100];  
    int seenCount = 0;

    Account* acc = head;

    while (acc != NULL) {
        Item* item = acc->items;

        while (item != NULL) {

            int exists = 0;
            for (int i = 0; i < seenCount; i++) {
                if (strcmp(seen[i], item->name) == 0) {
                    exists = 1;
                    break;
                }
            }

            if (!exists) {
                strcpy(seen[seenCount], item->name);
                seenCount++;
            }

            item = item->next;
        }

        acc = acc->next;
    }

    printf("\n=== Svi artikli u ponudi ===\n");
    for (int i = 0; i < seenCount; i++) {
        printf("- %s\n", seen[i]);
    }
    printf("=============================\n");

    return 0;
}

