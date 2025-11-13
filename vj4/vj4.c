#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define _CRT_SECURE_NO_WARNINGS

typedef struct Poly {
    int coef;
    int exp;
    struct Poly* next;
} Poly;

Poly* allocate(int coef, int exp);
int countRows(const char* filename);
int insertSorted(Poly* head, int coef, int exp);
Poly* readPoly(FILE* file);
void printPoly(Poly* head);
Poly* addPoly(Poly* p1, Poly* p2);
Poly* sumAll(FILE* file, int numOfPolys);
Poly* mulPoly(Poly* p1, Poly* p2);
Poly* productAll(FILE* file, int numOfPolys);

int main() {
    const char* filename = "poly.txt";
    int numOfPolys = countRows(filename);
    if (numOfPolys <= 0) return 1;

    FILE* file = fopen(filename, "r");
    if (!file) return 1;
    Poly* sum = sumAll(file, numOfPolys);
    rewind(file);
    if (!file) return 1;
    Poly* product = productAll(file, numOfPolys);
    fclose(file);

    return 0;
}

Poly* allocate(int coef, int exp) {
    Poly* newEl = (Poly*)malloc(sizeof(Poly));
    if (!newEl) {
        printf("Neuspjela alokacija memorije.\n");
        return NULL;
    }
    newEl->coef = coef;
    newEl->exp = exp;
    newEl->next = NULL;
    return newEl;
}

int countRows(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Neuspjesno otvaranje datoteke.\n");
        return EXIT_FAILURE;
    }
    int count = 0, ch, any = 0, last = '\n';
    while ((ch = fgetc(file)) != EOF) {
        any = 1;
        if (ch == '\n') count++;
        last = ch;
    }
    fclose(file);
    if (any && last != '\n') count++;
    if (count <= 0) {
        printf("Nema polinoma u datoteci.\n");
        return EXIT_FAILURE;
    }
    return count;
}

int insertSorted(Poly* head, int coef, int exp) {
    if (coef == 0) return 0;

    Poly* prev = head;
    Poly* current = head->next;

    while (current != NULL && current->exp > exp) {
        prev = current;
        current = current->next;
    }

    if (current != NULL && current->exp == exp) {
        current->coef += coef;
        if (current->coef == 0) {
            prev->next = current->next;
            free(current);
        }
        return 1;
    }

    Poly* newEl = allocate(coef, exp);
    prev->next = newEl;
    newEl->next = current;
    return 1;
}

Poly* readPoly(FILE* file) {
    Poly* head = allocate(0, 0);
    if (!head) return NULL;

    char line[2048];
    if (!fgets(line, sizeof(line), file)) return head;

    char* p = line;
    int c, e, n;
    while (sscanf(p, " %d %d %n", &c, &e, &n) == 2) {
        insertSorted(head, c, e);
        p += n;
    }
    return head;
}

void printPoly(Poly* head) {
    Poly* current = head->next;
    if (!current) {
        printf("Prazan polinom.\n");
        return;
    }
    while (current) {
        printf("%d x^%d  ", current->coef, current->exp);
        current = current->next;
    }
    printf("\n");
}

Poly* addPoly(Poly* p1, Poly* p2) {
    Poly* result = allocate(0, 0);
    if (!result) return NULL;

    Poly* a = p1->next;
    Poly* b = p2->next;

    while (a && b) {
        if (a->exp == b->exp) {
            insertSorted(result, a->coef + b->coef, a->exp);
            a = a->next;
            b = b->next;
        } else if (a->exp > b->exp) {
            insertSorted(result, a->coef, a->exp);
            a = a->next;
        } else {
            insertSorted(result, b->coef, b->exp);
            b = b->next;
        }
    }

    while (a) {
        insertSorted(result, a->coef, a->exp);
        a = a->next;
    }
    while (b) {
        insertSorted(result, b->coef, b->exp);
        b = b->next;
    }

    return result;
}

Poly* sumAll(FILE* file, int numOfPolys) {
    Poly* result = allocate(0, 0);
    if (!result) return NULL;

    for (int i = 0; i < numOfPolys; i++) {
        Poly* temp = readPoly(file);
        Poly* newResult = addPoly(result, temp);

        Poly* p = result;
        while (p) { Poly* q = p->next; free(p); p = q; }
        result = newResult;

        Poly* t = temp;
        while (t) { Poly* q = t->next; free(t); t = q; }
    }

    printf("\nZbroj svih polinoma:\n");
    printPoly(result);
    return result;
}

Poly* mulPoly(Poly* p1, Poly* p2) {
    Poly* result = allocate(0, 0);
    if (!result) return NULL;

    for (Poly* a = p1->next; a; a = a->next) {
        for (Poly* b = p2->next; b; b = b->next) {
            int coef = a->coef * b->coef;
            int exp = a->exp + b->exp;
            insertSorted(result, coef, exp);
        }
    }
    return result;
}

Poly* productAll(FILE* file, int numOfPolys) {
    Poly* result = allocate(0, 0);
    insertSorted(result, 1, 0);

    for (int i = 0; i < numOfPolys; i++) {
        Poly* temp = readPoly(file);
        Poly* newResult = mulPoly(result, temp);

        Poly* p = result;
        while (p) { Poly* q = p->next; free(p); p = q; }
        result = newResult;

        Poly* t = temp;
        while (t) { Poly* q = t->next; free(t); t = q; }
    }

    printf("\nUmnožak svih polinoma:\n");
    printPoly(result);
    return result;
}
