#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "racuni.h"
#define _CRT_SECURE_NO_WARNINGS

void clearInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {

    Account* all = loadAllAccounts("racuni/racuni.txt");
    if (all == NULL) {
        printf("Ne mogu ucitati racune.\n");
        return -1;
    }

    int choice = -1;

    while (choice != 0) {

        printf("\n================= IZBORNIK =================\n");
        printf("1. Ispisi sve racune\n");
        printf("2. Ukupna potrosnja artikla u periodu\n");
        printf("3. Ukupna kolicina artikla u periodu\n");
        printf("4. Ukupna potrosnja artikla (bez perioda)\n");
        printf("5. Ukupna kolicina artikla (bez perioda)\n");
        printf("6. Najvise kupljeni artikl\n");
        printf("7. Najvise potroseni artikl\n");
        printf("8. Izlistaj sve artikle u ponudi\n");
        printf("0. Izlaz\n");
        printf("============================================\n");
        printf("Unesite opciju: ");
        scanf("%d", &choice);
        clearInput();

        if (choice == 1) {
            printAccounts(all);
        }

        else if (choice == 2) {
            char item[100], from[15], to[15];

            printf("Unesite naziv artikla: ");
            fgets(item, sizeof(item), stdin);
            item[strcspn(item, "\n")] = '\0';

            printf("Unesite pocetni datum (YYYY-MM-DD): ");
            fgets(from, sizeof(from), stdin);
            from[strcspn(from, "\n")] = '\0';

            printf("Unesite krajnji datum (YYYY-MM-DD): ");
            fgets(to, sizeof(to), stdin);
            to[strcspn(to, "\n")] = '\0';

            double spent = calculateSpent(all, item, from, to);
            printf("\nUkupno potroseno na '%s' od %s do %s: %.2lf EUR\n",
                   item, from, to, spent);
        }

        else if (choice == 3) {
            char item[100], from[15], to[15];

            printf("Unesite naziv artikla: ");
            fgets(item, sizeof(item), stdin);
            item[strcspn(item, "\n")] = '\0';

            printf("Unesite pocetni datum (YYYY-MM-DD): ");
            fgets(from, sizeof(from), stdin);
            from[strcspn(from, "\n")] = '\0';

            printf("Unesite krajnji datum (YYYY-MM-DD): ");
            fgets(to, sizeof(to), stdin);
            to[strcspn(to, "\n")] = '\0';

            int qty = quantityInPeriod(all, item, from, to);
            printf("\nUkupno kupljeno '%s' od %s do %s: %d komada\n",
                   item, from, to, qty);
        }

        else if (choice == 4) {
            char item[100];

            printf("Unesite naziv artikla: ");
            fgets(item, sizeof(item), stdin);
            item[strcspn(item, "\n")] = '\0';

            double spent = totalSpentAll(all, item);
            printf("\nUkupno potroseno na '%s': %.2lf EUR\n", item, spent);
        }

        else if (choice == 5) {
            char item[100];

            printf("Unesite naziv artikla: ");
            fgets(item, sizeof(item), stdin);
            item[strcspn(item, "\n")] = '\0';

            int qty = quantityInAll(all, item);
            printf("\nUkupno kupljeno '%s': %d komada\n", item, qty);
        }

        else if (choice == 6) {
            char name[100];
            int qty;

            if (mostBoughtItem(all, name, &qty) == 0) {
                printf("\nNajvise kupljeni artikl: %s (%d komada)\n",
                       name, qty);
            }
        }

        else if (choice == 7) {
            char name[100];
            double money;

            if (mostSpentItem(all, name, &money) == 0) {
                printf("\nNajvise potroseno je na: %s (%.2lf EUR)\n",
                       name, money);
            }
        }

        else if (choice == 8) {
            listAllItems(all);
        }

        else if (choice == 0) {
            printf("Izlaz iz programa...\n");
        }

        else {
            printf("Nepostojeca opcija.\n");
        }
    }

    return 0;
}
