#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "fight.h"
#define _CRT_SECURE_NO_WARNINGS

Fighter* load_fighters(const char* filename)
{
    FILE* fp = fopen(filename, "r");
    Fighter* head = NULL;
    char line[512];

    if (fp == NULL) return NULL;

    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return NULL;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        int id, wins, losses, draws, ufc_fights;
        double win_rate;
        char name[NAME_LEN], nickname[NICK_LEN], wc[WC_LEN];

        char* token = strtok(line, ";");
        if (token == NULL) continue;
        id = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(name, token, NAME_LEN - 1);
        name[NAME_LEN - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(nickname, token, NICK_LEN - 1);
        nickname[NICK_LEN - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(wc, token, WC_LEN - 1);
        wc[WC_LEN - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        wins = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        losses = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        draws = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        win_rate = atof(token);

        token = strtok(NULL, ";\n");
        if (token == NULL) continue;
        ufc_fights = atoi(token);

        if (name[0] && name[strlen(name) - 1] == '\n') name[strlen(name) - 1] = '\0';
        if (nickname[0] && nickname[strlen(nickname) - 1] == '\n') nickname[strlen(nickname) - 1] = '\0';
        if (wc[0] && wc[strlen(wc) - 1] == '\n') wc[strlen(wc) - 1] = '\0';

        Fighter* f = fighter_create(id, name, nickname, wc, wins, losses, draws, win_rate, ufc_fights);
        head = fighter_append(head, f);
    }

    fclose(fp);
    return head;
}

int load_fights(const char* filename, Fighter* fighters)
{
    FILE* fp = fopen(filename, "r");
    char line[1024];
    int loaded = 0;

    if (fp == NULL) return 0;

    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return 0;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        int fighter_id, round, sig_landed, sig_taken;
        char opponent[OPP_LEN], event[EVENT_LEN], wc[WC_LEN], method[METHOD_LEN];
        char result;

        char* token = strtok(line, ";");
        if (token == NULL) continue;
        fighter_id = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(opponent, token, OPP_LEN - 1);
        opponent[OPP_LEN - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(event, token, EVENT_LEN - 1);
        event[EVENT_LEN - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(wc, token, WC_LEN - 1);
        wc[WC_LEN - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        result = token[0];

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(method, token, METHOD_LEN - 1);
        method[METHOD_LEN - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        round = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        sig_landed = atoi(token);

        token = strtok(NULL, ";\n");
        if (token == NULL) continue;
        sig_taken = atoi(token);

        if (opponent[0] && opponent[strlen(opponent) - 1] == '\n') opponent[strlen(opponent) - 1] = '\0';
        if (event[0] && event[strlen(event) - 1] == '\n') event[strlen(event) - 1] = '\0';
        if (wc[0] && wc[strlen(wc) - 1] == '\n') wc[strlen(wc) - 1] = '\0';
        if (method[0] && method[strlen(method) - 1] == '\n') method[strlen(method) - 1] = '\0';

        Fighter* f = fighter_find_by_id(fighters, fighter_id);
        if (f == NULL) continue;

        Fight* fight = fight_create(opponent, event, wc, result, method, round, sig_landed, sig_taken);
        f->fights_head = fight_append(f->fights_head, fight);

        loaded++;
    }

    fclose(fp);
    return loaded;
}
