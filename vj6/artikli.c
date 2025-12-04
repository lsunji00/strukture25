#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "racuni.h"   

Item* createItem(const char* name, int qty, double price) {
    Item* newItem = malloc(sizeof(Item));
    if (!newItem) {
        printf("Problem alokacije memorije(Item)\n");
        return NULL;
    }

    strcpy(newItem->name, name);
    newItem->quantity = qty;
    newItem->price = price;
    newItem->next = NULL;

    return newItem;
}
Item* insertItemSorted(Item* head, Item* newItem) {
    if (newItem == NULL) {
        return head; 
    }

    if (head == NULL || strcmp(newItem->name, head->name) < 0) {
        newItem->next = head;
        return newItem;
    }

    Item* current = head;

    while (current->next != NULL &&
           strcmp(newItem->name, current->next->name) > 0) {
        current = current->next;
    }

    newItem->next = current->next;
    current->next = newItem;

    return head; 
}

