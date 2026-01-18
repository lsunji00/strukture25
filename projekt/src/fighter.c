#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fighter.h"
#include "fight.h"
#define _CRT_SECURE_NO_WARNINGS

Fighter* fighter_create(int id,
                        const char* name,
                        const char* nickname,
                        const char* weight_class,
                        int wins, int losses, int draws,
                        double win_rate,
                        int ufc_fights)
{
    Fighter* f = (Fighter*)malloc(sizeof(Fighter));
    if (f == NULL) return NULL;

    f->id = id;

    strncpy(f->name, name, NAME_LEN - 1);
    f->name[NAME_LEN - 1] = '\0';

    strncpy(f->nickname, nickname, NICK_LEN - 1);
    f->nickname[NICK_LEN - 1] = '\0';

    strncpy(f->weight_class, weight_class, WC_LEN - 1);
    f->weight_class[WC_LEN - 1] = '\0';

    f->wins = wins;
    f->losses = losses;
    f->draws = draws;
    f->win_rate = win_rate;
    f->ufc_fights = ufc_fights;

    f->fights_head = NULL;
    f->next = NULL;

    return f;
}

Fighter* fighter_append(Fighter* head, Fighter* node)
{
    Fighter* curr = NULL;

    if (node == NULL) return head;
    if (head == NULL) return node;

    curr = head;
    while (curr->next != NULL)
        curr = curr->next;

    curr->next = node;
    return head;
}

Fighter* fighter_find_by_id(Fighter* head, int id)
{
    Fighter* curr = head;

    while (curr != NULL) {
        if (curr->id == id) return curr;
        curr = curr->next;
    }

    return NULL;
}

int fighter_count(Fighter* head)
{
    int count = 0;
    Fighter* curr = head;

    while (curr != NULL) {
        count++;
        curr = curr->next;
    }

    return count;
}

int fighter_print_one(const Fighter* f)
{
    if (f == NULL) return 0;

    printf("%d | %s | %s | %s | %d-%d-%d | WR: %.3f | UFC fights: %d\n",
           f->id, f->name, f->nickname, f->weight_class,
           f->wins, f->losses, f->draws, f->win_rate, f->ufc_fights);

    return 1;
}

int fighter_print_first_n(Fighter* head, int n)
{
    int printed = 0;
    Fighter* curr = head;

    if (n <= 0) return 0;

    while (curr != NULL && printed < n) {
        if (fighter_print_one(curr) == 1)
            printed++;
        curr = curr->next;
    }

    return printed;
}

int fighter_free_all(Fighter* head)
{
    int freed = 0;
    Fighter* curr = head;
    Fighter* next = NULL;

    while (curr != NULL) {
        next = curr->next;
        fight_free_all(curr->fights_head);
        free(curr);
        curr = next;
        freed++;
    }

    return freed;
}
