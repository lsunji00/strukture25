#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "fight.h"
#include "fighter.h"
#include "weightclass.h"
#include "report.h"
#define _CRT_SECURE_NO_WARNINGS

static int read_int(void)
{
    int x = 0;
    if (scanf("%d", &x) != 1) return -1;
    return x;
}

static int fighter_print_fights_first_n(Fighter* f, int n)
{
    int printed = 0;
    Fight* curr = NULL;

    if (f == NULL || n <= 0) return 0;

    curr = f->fights_head;
    while (curr != NULL && printed < n) {
        printf("  vs %-25s | %c | %-8s | R%d | SL %d | ST %d | %s\n",
               curr->opponent,
               curr->result,
               curr->method,
               curr->round,
               curr->sig_landed,
               curr->sig_taken,
               curr->event);
        printed++;
        curr = curr->next;
    }

    return printed;
}

static int fighter_print_by_id(Fighter* head, int id)
{
    Fighter* f = fighter_find_by_id(head, id);
    if (f == NULL) return 0;

    fighter_print_one(f);
    printf("Borbe (prvih 10):\n");
    fighter_print_fights_first_n(f, 10);

    return 1;
}

static int fighter_strike_diff(Fighter* f)
{
    int diff = 0;
    Fight* curr = NULL;

    if (f == NULL) return 0;

    curr = f->fights_head;
    while (curr != NULL) {
        diff += (curr->sig_landed - curr->sig_taken);
        curr = curr->next;
    }

    return diff;
}

static int print_top_n_by_strike_diff(Fighter* head, int n)
{
    int i = 0, count = 0;
    Fighter* curr = NULL;

    Fighter* topF[100];
    int topD[100];

    if (n <= 0) return 0;
    if (n > 100) n = 100;

    for (i = 0; i < n; i++) {
        topF[i] = NULL;
        topD[i] = -2147483647;
    }

    curr = head;
    while (curr != NULL) {
        int d = fighter_strike_diff(curr);

        int pos = -1;
        for (i = 0; i < n; i++) {
            if (d > topD[i]) {
                pos = i;
                break;
            }
        }

        if (pos != -1) {
            for (i = n - 1; i > pos; i--) {
                topD[i] = topD[i - 1];
                topF[i] = topF[i - 1];
            }
            topD[pos] = d;
            topF[pos] = curr;
        }

        curr = curr->next;
    }

    printf("TOP %d po strike differential (sig_landed - sig_taken):\n", n);
    for (i = 0; i < n; i++) {
        if (topF[i] != NULL) {
            printf("%2d) %s | diff: %d\n", i + 1, topF[i]->name, topD[i]);
            count++;
        }
    }

    return count;
}

static int print_top_n_by_win_rate(Fighter* head, int n)
{
    int i = 0, count = 0;
    Fighter* curr = NULL;

    Fighter* topF[100];
    double topW[100];

    if (n <= 0) return 0;
    if (n > 100) n = 100;

    for (i = 0; i < n; i++) {
        topF[i] = NULL;
        topW[i] = -1.0;
    }

    curr = head;
    while (curr != NULL) {
        double w = curr->win_rate;

        int pos = -1;
        for (i = 0; i < n; i++) {
            if (w > topW[i]) {
                pos = i;
                break;
            }
            if (w == topW[i] && topF[i] != NULL && curr->wins > topF[i]->wins) {
            pos = i;
            break;
            }
        }

        if (pos != -1) {
            for (i = n - 1; i > pos; i--) {
                topW[i] = topW[i - 1];
                topF[i] = topF[i - 1];
            }
            topW[pos] = w;
            topF[pos] = curr;
        }

        curr = curr->next;
    }

    printf("TOP %d po win_rate:\n", n);
    for (i = 0; i < n; i++) {
        if (topF[i] != NULL) {
            printf("%2d) %s | win_rate: %.3f | %d-%d-%d\n",
                   i + 1,
                   topF[i]->name,
                   topW[i],
                   topF[i]->wins, topF[i]->losses, topF[i]->draws);
            count++;
        }
    }

    return count;
}
static int fighter_total_sig_landed(Fighter* f)
{
    int sum = 0;
    Fight* curr = NULL;

    if (f == NULL) return 0;

    curr = f->fights_head;
    while (curr != NULL) {
        sum += curr->sig_landed;
        curr = curr->next;
    }

    return sum;
}

static int fighter_total_sig_taken(Fighter* f)
{
    int sum = 0;
    Fight* curr = NULL;

    if (f == NULL) return 0;

    curr = f->fights_head;
    while (curr != NULL) {
        sum += curr->sig_taken;
        curr = curr->next;
    }

    return sum;
}
static int print_top_n_by_strike_ratio(Fighter* head, int n)
{
    int i = 0, count = 0;
    Fighter* curr = NULL;

    Fighter* topF[100];
    double topR[100];

    if (n <= 0) return 0;
    if (n > 100) n = 100;

    for (i = 0; i < n; i++) {
        topF[i] = NULL;
        topR[i] = -1.0;
    }

    curr = head;
    while (curr != NULL) {
        int landed = fighter_total_sig_landed(curr);
        int taken  = fighter_total_sig_taken(curr);
        double r = 0.0;

        if (taken == 0) {
            if (landed > 0) r = 999999.0;
            else r = 0.0;
        } else {
            r = (double)landed / (double)taken;
        }

        {
            int pos = -1;
            for (i = 0; i < n; i++) {
                if (r > topR[i]) {
                    pos = i;
                    break;
                }
            }

            if (pos != -1) {
                for (i = n - 1; i > pos; i--) {
                    topR[i] = topR[i - 1];
                    topF[i] = topF[i - 1];
                }
                topR[pos] = r;
                topF[pos] = curr;
            }
        }

        curr = curr->next;
    }

    printf("TOP %d po strike omjeru (total_landed / total_taken):\n", n);
    for (i = 0; i < n; i++) {
        if (topF[i] != NULL) {
            int landed = fighter_total_sig_landed(topF[i]);
            int taken  = fighter_total_sig_taken(topF[i]);
            printf("%2d) %s | ratio: %.3f | landed: %d | taken: %d\n",
                   i + 1,
                   topF[i]->name,
                   topR[i],
                   landed,
                   taken);
            count++;
        }
    }

    return count;
}


int menu_run(Fighter* fighters)
{
    int choice = -1;

    while (choice != 0) {
        printf("\n=== UFC Rang Projekt ===\n");
        printf("1) Prikazi borca po ID-u (i prvih 10 borbi)\n");
        printf("2) Top N boraca po win_rate\n");
        printf("3) Top N boraca po strike differential\n");
        printf("4) Statistika po weight classu\n");
        printf("5) Top N boraca po strike omjeru\n");
        printf("6) Generiraj report.txt\n");
        printf("0) Izlaz\n");
        printf("Odabir: ");

        choice = read_int();
        if (choice == -1) return 0;

        if (choice == 1) {
            int id = 0;
            printf("Unesi fighter ID: ");
            id = read_int();
            if (fighter_print_by_id(fighters, id) == 0)
                printf("Nema borca s ID %d\n", id);
        }
        else if (choice == 2) {
            int n = 0;
            printf("Unesi N: ");
            n = read_int();
            print_top_n_by_win_rate(fighters, n);
        }
        else if (choice == 3) {
            int n = 0;
            printf("Unesi N: ");
            n = read_int();
            print_top_n_by_strike_diff(fighters, n);
        }
        else if (choice == 4) {
        int wc_choice = -1;
        const char* wc_name = NULL;

        printf("Odaberi weight class:\n");
        printf("1) Flyweight Bout\n");
        printf("2) Bantamweight Bout\n");
        printf("3) Featherweight Bout\n");
        printf("4) Lightweight Bout\n");
        printf("5) Welterweight Bout\n");
        printf("6) Middleweight Bout\n");
        printf("7) Light Heavyweight Bout\n");
        printf("8) Heavyweight Bout\n");
        printf("Odabir: ");

        wc_choice = read_int();

        if (wc_choice == 1) wc_name = "Flyweight Bout";
        else if (wc_choice == 2) wc_name = "Bantamweight Bout";
        else if (wc_choice == 3) wc_name = "Featherweight Bout";
        else if (wc_choice == 4) wc_name = "Lightweight Bout";
        else if (wc_choice == 5) wc_name = "Welterweight Bout";
        else if (wc_choice == 6) wc_name = "Middleweight Bout";
        else if (wc_choice == 7) wc_name = "Light Heavyweight Bout";
        else if (wc_choice == 8) wc_name = "Heavyweight Bout";

        if (wc_name == NULL) {
            printf("Nepoznata kategorija.\n");
        } 
        else {
            WeightClassStats* one = wc_build_single_from_fighters(fighters, wc_name);
            wc_print_one(one);
            wc_free_all(one);
        }
        }
        else if (choice == 5) {
            int n = 0;
            printf("Unesi N: ");
            n = read_int();
            print_top_n_by_strike_ratio(fighters, n);
        }
        else if (choice == 6) {
            if (report_generate("report.txt", fighters) == 1)
                printf("Report generiran: report.txt\n");
            else
                printf("Greska: report nije generiran.\n");
        }
        else if (choice == 0) {
            return 1;
        }
        else {
            printf("Nepoznata opcija.\n");
        }
    }

    return 1;
}
