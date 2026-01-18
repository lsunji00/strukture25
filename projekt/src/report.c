#include <stdio.h>
#include <string.h>
#include "report.h"
#include "fight.h"
#include "weightclass.h"
#define _CRT_SECURE_NO_WARNINGS

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

static double fighter_strike_ratio(Fighter* f)
{
    int landed = fighter_total_sig_landed(f);
    int taken  = fighter_total_sig_taken(f);

    if (taken == 0) {
        if (landed > 0) return 999999.0;
        return 0.0;
    }
    return (double)landed / (double)taken;
}

static int write_top_n_by_win_rate(FILE* out, Fighter* head, int n)
{
    int i, count = 0;
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
            if (w > topW[i]) { pos = i; break; }
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

    fprintf(out, "TOP %d po win_rate:\n", n);
    for (i = 0; i < n; i++) {
        if (topF[i] != NULL) {
            fprintf(out, "%2d) %s | win_rate: %.3f | %d-%d-%d\n",
                    i + 1,
                    topF[i]->name,
                    topW[i],
                    topF[i]->wins, topF[i]->losses, topF[i]->draws);
            count++;
        }
    }
    fprintf(out, "\n");
    return count;
}

static int write_top_n_by_strike_ratio(FILE* out, Fighter* head, int n)
{
    int i, count = 0;
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
        double r = fighter_strike_ratio(curr);
        int pos = -1;

        for (i = 0; i < n; i++) {
            if (r > topR[i]) { pos = i; break; }
        }

        if (pos != -1) {
            for (i = n - 1; i > pos; i--) {
                topR[i] = topR[i - 1];
                topF[i] = topF[i - 1];
            }
            topR[pos] = r;
            topF[pos] = curr;
        }

        curr = curr->next;
    }

    fprintf(out, "TOP %d po strike omjeru (total_landed / total_taken):\n", n);
    for (i = 0; i < n; i++) {
        if (topF[i] != NULL) {
            int landed = fighter_total_sig_landed(topF[i]);
            int taken  = fighter_total_sig_taken(topF[i]);
            fprintf(out, "%2d) %s | ratio: %.3f | landed: %d | taken: %d\n",
                    i + 1,
                    topF[i]->name,
                    topR[i],
                    landed, taken);
            count++;
        }
    }
    fprintf(out, "\n");
    return count;
}

static int write_top_n_by_strike_diff(FILE* out, Fighter* head, int n)
{
    int i, count = 0;
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
            if (d > topD[i]) { pos = i; break; }
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

    fprintf(out, "TOP %d po strike differential (total_landed - total_taken):\n", n);
    for (i = 0; i < n; i++) {
        if (topF[i] != NULL) {
            int landed = fighter_total_sig_landed(topF[i]);
            int taken  = fighter_total_sig_taken(topF[i]);
            fprintf(out, "%2d) %s | diff: %d | landed: %d | taken: %d\n",
                    i + 1,
                    topF[i]->name,
                    topD[i],
                    landed, taken);
            count++;
        }
    }
    fprintf(out, "\n");
    return count;
}

static int write_weightclass_stats(FILE* out, Fighter* fighters)
{
    int count = 0;
    WeightClassStats* head = wc_build_from_fighters(fighters);
    WeightClassStats* curr = head;

    fprintf(out, "STATISTIKA PO WEIGHT CLASS:\n");

    while (curr != NULL) {
        double real_fights = (double)curr->entries_count / 2.0;
        double finish_rate = (curr->entries_count > 0)
            ? ((double)(curr->ko_tko + curr->sub) / (double)curr->entries_count)
            : 0.0;

        double avg_sig_landed = (curr->entries_count > 0)
            ? ((double)curr->sig_landed_total / (double)curr->entries_count)
            : 0.0;

        double avg_sig_taken = (curr->entries_count > 0)
            ? ((double)curr->sig_taken_total / (double)curr->entries_count)
            : 0.0;

        double avg_action = avg_sig_landed + avg_sig_taken;

        fprintf(out,
                "%-20s | borci: %4d | fights~: %6.1f | KO: %4d SUB: %4d DEC: %4d | finish: %.3f | avg action: %.2f\n",
                curr->name,
                curr->fighters_count,
                real_fights,
                curr->ko_tko, curr->sub, curr->dec,
                finish_rate,
                avg_action);

        count++;
        curr = curr->next;
    }

    fprintf(out, "\n");

    wc_free_all(head);
    return count;
}

int report_generate(const char* filename, Fighter* fighters)
{
    FILE* out = NULL;

    if (filename == NULL || fighters == NULL) return 0;

    out = fopen(filename, "w");
    if (out == NULL) return 0;

    fprintf(out, "UFC Rang Projekt - REPORT\n\n");
    fprintf(out, "Broj boraca: %d\n\n", fighter_count(fighters));

    write_weightclass_stats(out, fighters);

    write_top_n_by_win_rate(out, fighters, 10);
    write_top_n_by_strike_ratio(out, fighters, 10);
    write_top_n_by_strike_diff(out, fighters, 10);

    fclose(out);
    return 1;
}
