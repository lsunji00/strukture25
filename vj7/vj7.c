#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Directory {
    char name[50];
    struct Directory* child;
    struct Directory* sibling;
} Directory;

typedef struct StackNode {
    Directory* dir;
    struct StackNode* next;
} StackNode;

Directory* createDirectory(const char* name);
int push(StackNode* head, Directory* dir);
Directory* pop(StackNode* head);
int makeDirectory(Directory* current, const char* name);
Directory* changeDirectory(Directory* current, StackNode* head, const char* name);
Directory* changeToParent(Directory* current, StackNode* head);
int listDirectory(Directory* current);
int printTree(Directory* current, int level);
int freeStack(StackNode* head);
int freeTree(Directory* current);

int main() {
    setbuf(stdout, NULL);

    Directory* root = createDirectory("root");
    Directory* current = root;

    StackNode stackHead;
    stackHead.next = NULL;

    char command[50];
    char name[50];

    while (1) {
        printf("\n===== IZBORNIK =====\n");
        printf("1) md <ime>\n");
        printf("2) cd <ime>\n");
        printf("3) cd ..\n");
        printf("4) dir\n");
        printf("5) tree\n");
        printf("6) exit\n");
        printf("Unesite naredbu: ");

        scanf("%s", command);

        if (strcmp(command, "md") == 0) {
            scanf("%s", name);
            int r = makeDirectory(current, name);
            if (r == 1) printf("Direktorij %s je kreiran.\n", name);
            else if (r == 0) printf("Direktorij %s vec postoji.\n", name);
            else printf("Greska pri kreiranju direktorija.\n");
        }

        else if (strcmp(command, "cd") == 0) {
            scanf("%s", name);

            if (strcmp(name, "..") == 0) {
                current = changeToParent(current, &stackHead);
            } 
            else {
                Directory* newDir = changeDirectory(current, &stackHead, name);
                if (newDir != NULL)
                    current = newDir;
            }
        }

        else if (strcmp(command, "dir") == 0) {
            listDirectory(current);
        }

        else if (strcmp(command, "tree") == 0) {
            printTree(root, 0);
        }

        else if (strcmp(command, "exit") == 0) {
            freeStack(&stackHead);
            freeTree(root);
            return 0;
        }

        else {
            printf("Nepoznata naredba.\n");
        }
    }

    return 0;
}

int push(StackNode* head, Directory* dir) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->dir = dir;
    newNode->next = head->next;
    head->next = newNode;
    return 0;
}
Directory* pop(StackNode* head) {
    if (head->next == NULL)
        return NULL;

    StackNode* first = head->next;
    Directory* d = first->dir;

    head->next = first->next;
    free(first);

    return d;
}
Directory* createDirectory(const char* name) {
    Directory* newDir = (Directory*)malloc(sizeof(Directory));
    if (!newDir)
        return NULL;

    strcpy(newDir->name, name);
    newDir->child = NULL;
    newDir->sibling = NULL;
    return newDir;
}
int makeDirectory(Directory* current, const char* name) {
    if (current->child == NULL) {
        current->child = createDirectory(name);
        if (!current->child)
            return -1;
        return 1;
    }

    Directory* temp = current->child;

    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0)
            return 0;

        if (temp->sibling == NULL)
            break;

        temp = temp->sibling;
    }

    temp->sibling = createDirectory(name);
    if (!temp->sibling)
        return -1;

    return 1;
}
Directory* changeDirectory(Directory* current, StackNode* head, const char* name) {
    Directory* temp = current->child;

    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            push(head, current);
            return temp;
        }
        temp = temp->sibling;
    }

    printf("Direktorij %s ne postoji u ovom direktoriju.\n", name);
    return NULL;
}
Directory* changeToParent(Directory* current, StackNode* head) {
    if (head->next == NULL)
        return current;

    Directory* parent = pop(head);
    return parent;
}
int listDirectory(Directory* current) {
    Directory* temp = current->child;

    if (temp == NULL) {
        printf("Direktorij je prazan.\n");
        return 0;
    }

    while (temp != NULL) {
        printf("%s\n", temp->name);
        temp = temp->sibling;
    }

    return 0;
}
int printTree(Directory* current, int level) {
    if (current == NULL)
        return 0;

    for (int i = 0; i < level; i++)
        printf("  ");

    printf("%s\n", current->name);

    printTree(current->child, level + 1);
    printTree(current->sibling, level);

    return 0;
}
int freeStack(StackNode* head) {
    StackNode* temp = head->next;
    while (temp != NULL) {
        StackNode* next = temp->next;
        free(temp);
        temp = next;
    }
    head->next = NULL;
    return 0;
}
int freeTree(Directory* current) {
    if (current == NULL)
        return 0;

    freeTree(current->child);
    freeTree(current->sibling);

    free(current);
    return 0;
}





