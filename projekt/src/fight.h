#ifndef FIGHT_H
#define FIGHT_H
#define _CRT_SECURE_NO_WARNINGS
#define OPP_LEN 64
#define EVENT_LEN 128
#define WC_LEN 32
#define METHOD_LEN 16

typedef struct Fight {
    char opponent[OPP_LEN];
    char event[EVENT_LEN];
    char weight_class[WC_LEN];
    char result;
    char method[METHOD_LEN];
    int round;
    int sig_landed;
    int sig_taken;

    struct Fight* next;
} Fight;

Fight* fight_create(const char* opponent,
                    const char* event,
                    const char* weight_class,
                    char result,
                    const char* method,
                    int round,
                    int sig_landed,
                    int sig_taken);

Fight* fight_append(Fight* head, Fight* node);

int fight_free_all(Fight* head);

#endif
