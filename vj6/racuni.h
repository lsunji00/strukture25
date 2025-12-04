#ifndef RACUNI_H
#define RACUNI_H

typedef struct Item {
    char name[100];
    int quantity;
    double price;
    struct Item* next;
} Item;

typedef struct Account {
    char date[11];
    Item* items;
    struct Account* next;
} Account;

Account* loadAllAccounts(const char* path);
Account* loadSingleAccount(const char* filename);
Item* insertItemSorted(Item* head, Item* newItem);
int printAccounts(Account* head);
double totalSpentAll(Account* head, const char* itemName);
Item* createItem(const char* name, int qty, double price);
Account* insertAccountSorted(Account* head, Account* newAcc);
double calculateSpent(Account* head, const char* itemName, const char* fromDate, const char* toDate);
int quantityInPeriod(Account* head, const char* itemName,const char* fromDate, const char* toDate);
int quantityInAll(Account* head, const char* itemName);
int mostBoughtItem(Account* head, char* resultName, int* resultQty);
int mostSpentItem(Account* head, char* resultName, double* resultMoney);
int listAllItems(Account* head);

#endif
