#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "weightclass.h"
#include "fight.h"
#define _CRT_SECURE_NO_WARNINGS

static int method_is_ko(const char* m)
{
    return (strcmp(m, "KO/TKO") == 0);
}

static int method_is_sub(const char* m)
{
    return (strcmp(m, "Submission") == 0);
}

static int method_is_dec(const char* m)
{
    if (m == NULL) return 0;
    return (strstr(m, "Decision") != NULL);
}

WeightClassStats* wc_create(const char* name)
{
    WeightClassStats* wc = (WeightClassStats*)malloc(sizeof(WeightClassStats));
    if (wc == NULL) return NULL;

    strncpy(wc->name, name, WC_NAME_LEN - 1);
    wc->name[WC_NAME_LEN - 1] = '\0';

    wc->fighters_count = 0;
    wc->entries_count = 0;

    wc->ko_tko = 0;
    wc->sub = 0;
    wc->dec = 0;

    wc->sig_landed_total = 0;
    wc->sig_taken_total = 0;

    wc->next = NULL;

    return wc;
}

WeightClassStats* wc_find(WeightClassStats* head, const char* name)
{
    WeightClassStats* curr = head;
    while (curr != NULL) {
        if (strcmp(curr->name, name) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

WeightClassStats* wc_get_or_add(WeightClassStats* head, const char* name)
{
    WeightClassStats* found = wc_find(head, name);
    if (found != NULL) return head;

    WeightClassStats* node = wc_create(name);
    if (node == NULL) return head;

    node->next = head;
    return node;
}

int wc_add_fighter(WeightClassStats* wc)
{
    if (wc == NULL) return 0;
    wc->fighters_count++;
    return 1;
}

int wc_add_fight_entry(WeightClassStats* wc,
                       const char* method,
                       int sig_landed,
                       int sig_taken)
{
    if (wc == NULL) return 0;

    wc->entries_count++;

    if (method_is_ko(method)) wc->ko_tko++;
    else if (method_is_sub(method)) wc->sub++;
    else if (method_is_dec(method)) wc->dec++;

    wc->sig_landed_total += sig_landed;
    wc->sig_taken_total += sig_taken;

    return 1;
}

WeightClassStats* wc_build_from_fighters(Fighter* fighters)
{
    Fighter* f = fighters;
    WeightClassStats* head = NULL;

    while (f != NULL) {
        if (f->weight_class[0] != '\0') {
            head = wc_get_or_add(head, f->weight_class);
            WeightClassStats* wc = wc_find(head, f->weight_class);
            wc_add_fighter(wc);

            Fight* fight = f->fights_head;
            while (fight != NULL) {
                wc_add_fight_entry(wc, fight->method, fight->sig_landed, fight->sig_taken);
                fight = fight->next;
            }
        }
        f = f->next;
    }

    return head;
}

int wc_print_all(WeightClassStats* head)
{
    WeightClassStats* curr = head;
    int count = 0;

    while (curr != NULL) {
        double real_fights = (double)curr->entries_count / 2.0;
        double finish_total = (double)(curr->ko_tko + curr->sub);
        double finish_rate = (curr->entries_count > 0) ? (finish_total / (double)curr->entries_count) : 0.0;

        double avg_sig_landed = (curr->entries_count > 0) ? ((double)curr->sig_landed_total / (double)curr->entries_count) : 0.0;
        double avg_sig_taken  = (curr->entries_count > 0) ? ((double)curr->sig_taken_total  / (double)curr->entries_count) : 0.0;
        double avg_action = avg_sig_landed + avg_sig_taken;

        printf("%-20s | borci: %4d | fights~: %6.1f | KO: %4d SUB: %4d DEC: %4d | finish: %.3f | avg action: %.2f\n",
               curr->name,
               curr->fighters_count,
               real_fights,
               curr->ko_tko, curr->sub, curr->dec,
               finish_rate,
               avg_action);

        count++;
        curr = curr->next;
    }

    return count;
}

int wc_free_all(WeightClassStats* head)
{
    int freed = 0;
    WeightClassStats* curr = head;
    WeightClassStats* next = NULL;

    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
        freed++;
    }

    return freed;
}

WeightClassStats* wc_build_single_from_fighters(Fighter* fighters, const char* wc_name)
{
    Fighter* f = fighters;
    WeightClassStats* wc = wc_create(wc_name);

    if (wc == NULL) return NULL;

    while (f != NULL) {
        if (strcmp(f->weight_class, wc_name) == 0) {
            wc_add_fighter(wc);

            Fight* fight = f->fights_head;
            while (fight != NULL) {
                wc_add_fight_entry(wc, fight->method, fight->sig_landed, fight->sig_taken);
                fight = fight->next;
            }
        }
        f = f->next;
    }

    return wc;
}

int wc_print_one(WeightClassStats* wc)
{
    double real_fights;
    double finish_rate;
    double avg_sig_landed;
    double avg_sig_taken;
    double avg_action;

    if (wc == NULL) return 0;

    real_fights = (double)wc->entries_count / 2.0;
    finish_rate = (wc->entries_count > 0) ? ((double)(wc->ko_tko + wc->sub) / (double)wc->entries_count) : 0.0;

    avg_sig_landed = (wc->entries_count > 0) ? ((double)wc->sig_landed_total / (double)wc->entries_count) : 0.0;
    avg_sig_taken  = (wc->entries_count > 0) ? ((double)wc->sig_taken_total  / (double)wc->entries_count) : 0.0;
    avg_action = avg_sig_landed + avg_sig_taken;

    printf("%-20s | borci: %4d | fights~: %6.1f | KO: %4d SUB: %4d DEC: %4d | finish: %.3f | avg action: %.2f\n",
           wc->name,
           wc->fighters_count,
           real_fights,
           wc->ko_tko, wc->sub, wc->dec,
           finish_rate,
           avg_action);

    return 1;
}
