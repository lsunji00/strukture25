#include <stdio.h>
#include "io.h"
#include "fighter.h"
#include "menu.h"
#define _CRT_SECURE_NO_WARNINGS

int main(void)
{
    Fighter* fighters = load_fighters("data/fighter.txt");
    int fights_loaded = 0;

    if (fighters == NULL) {
        printf("Greska: ne mogu ucitati data/fighter.txt\n");
        return 0;
    }

    fights_loaded = load_fights("data/fights.txt", fighters);

    printf("Ucitan broj boraca: %d\n", fighter_count(fighters));
    printf("Ucitan broj fight zapisa: %d\n", fights_loaded);

    menu_run(fighters);

    fighter_free_all(fighters);
    return 0;
}
