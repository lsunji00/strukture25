#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fight.h"
#define _CRT_SECURE_NO_WARNINGS

Fight* fight_create(const char* opponent,
                    const char* event,
                    const char* weight_class,
                    char result,
                    const char* method,
                    int round,
                    int sig_landed,
                    int sig_taken)
{
    Fight* f = (Fight*)malloc(sizeof(Fight));
    if (f == NULL) return NULL;

    strncpy(f->opponent, opponent, OPP_LEN - 1);
    f->opponent[OPP_LEN - 1] = '\0';

    strncpy(f->event, event, EVENT_LEN - 1);
    f->event[EVENT_LEN - 1] = '\0';

    strncpy(f->weight_class, weight_class, WC_LEN - 1);
    f->weight_class[WC_LEN - 1] = '\0';

    f->result = result;

    strncpy(f->method, method, METHOD_LEN - 1);
    f->method[METHOD_LEN - 1] = '\0';

    f->round = round;
    f->sig_landed = sig_landed;
    f->sig_taken = sig_taken;

    f->next = NULL;

    return f;
}

Fight* fight_append(Fight* head, Fight* node)
{
    Fight* curr = NULL;

    if (node == NULL) return head;
    if (head == NULL) return node;

    curr = head;
    while (curr->next != NULL)
        curr = curr->next;

    curr->next = node;
    return head;
}

int fight_free_all(Fight* head)
{
    int freed = 0;
    Fight* curr = head;
    Fight* next = NULL;

    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
        freed++;
    }

    return freed;
}
