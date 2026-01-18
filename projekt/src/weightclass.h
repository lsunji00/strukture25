#ifndef WEIGHTCLASS_H
#define WEIGHTCLASS_H

#include "fighter.h"

#define WC_NAME_LEN 32

typedef struct WeightClassStats {
    char name[WC_NAME_LEN];

    int fighters_count;
    int entries_count;

    int ko_tko;
    int sub;
    int dec;

    int sig_landed_total;
    int sig_taken_total;

    struct WeightClassStats* next;
} WeightClassStats;

WeightClassStats* wc_create(const char* name);
WeightClassStats* wc_find(WeightClassStats* head, const char* name);
WeightClassStats* wc_get_or_add(WeightClassStats* head, const char* name);

int wc_add_fighter(WeightClassStats* wc);

int wc_add_fight_entry(WeightClassStats* wc,
                       const char* method,
                       int sig_landed,
                       int sig_taken);

WeightClassStats* wc_build_from_fighters(Fighter* fighters);

int wc_print_all(WeightClassStats* head);

int wc_free_all(WeightClassStats* head);

WeightClassStats* wc_build_single_from_fighters(Fighter* fighters, const char* wc_name);

int wc_print_one(WeightClassStats* wc);


#endif
