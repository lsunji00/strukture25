#ifndef FIGHTER_H
#define FIGHTER_H

#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS
#define NAME_LEN 64
#define NICK_LEN 64
#define WC_LEN 32

typedef struct Fight Fight;

typedef struct Fighter {
    int id;

    char name[NAME_LEN];
    char nickname[NICK_LEN];
    char weight_class[WC_LEN];

    int wins;
    int losses;
    int draws;
    double win_rate;
    int ufc_fights;

    Fight* fights_head;

    struct Fighter* next;
} Fighter;

Fighter* fighter_create(int id,
                        const char* name,
                        const char* nickname,
                        const char* weight_class,
                        int wins, int losses, int draws,
                        double win_rate,
                        int ufc_fights);

Fighter* fighter_append(Fighter* head, Fighter* node);

Fighter* fighter_find_by_id(Fighter* head, int id);

int fighter_count(Fighter* head);

int fighter_print_one(const Fighter* f);
int fighter_print_first_n(Fighter* head, int n);

int fighter_free_all(Fighter* head);

#endif
