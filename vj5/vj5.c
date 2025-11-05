#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

typedef struct Position{
    double num;
    struct Position* next;
} Position;

int error(const char* msg);
Position* allocate(double num);
int push(Position* head, double num);
int pop(Position* head, double* result);
double calculatePostfix(Position* head, const char* filename);
int freeAll(Position* head);

int main(){

    Position* head=(Position*)malloc(sizeof(Position));
    if(!head) return error("Neuspjesna alokacija headera.");
    head->next=NULL;

    double result = calculatePostfix(head, "postfix.txt");
    printf("Rezultat: %.2lf\n", result);

    return freeAll(head);
}
int error(const char* msg){
    printf("%s",msg);
    return EXIT_FAILURE;
}
Position* allocate(double num){
    Position* newPosition=(Position*)malloc(sizeof(Position));
    if(!newPosition){
        return NULL;
    }
    newPosition->num=num;
    newPosition->next=NULL;
    return newPosition;
}
int push(Position* head, double num){
    Position* newNode = allocate(num);
    if (!newNode) return EXIT_FAILURE;

    newNode->next = head->next;
    head->next = newNode;
    return EXIT_SUCCESS;
}
int pop(Position* head, double* result) {
    if (head->next == NULL) return error("Stack is empty.\n");

    Position* temp = head->next;
    *result = temp->num;
    head->next = temp->next;
    free(temp);
    return EXIT_SUCCESS;
}
double calculatePostfix(Position* head, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        error("File could not be opened.\n");
        return EXIT_FAILURE;
    }

    char buffer[128];
    while (fscanf(file, "%s", buffer) == 1) {
        // Ako je broj ili negativan broj
        if (isdigit(buffer[0]) || (buffer[0] == '-' && isdigit(buffer[1]))) {
            double num = atof(buffer);
            push(head, num);
        }
        // Ako je operator
        else {
            double b, a;
            if (pop(head, &b) != EXIT_SUCCESS || pop(head, &a) != EXIT_SUCCESS) {
                fclose(file);
                return 0;
            }

            switch (buffer[0]) {
                case '+': push(head, a + b); break;
                case '-': push(head, a - b); break;
                case '*': push(head, a * b); break;
                case '/':
                    if (b == 0) {
                        fclose(file);
                        return error("Division by zero.\n");
                    }
                    push(head, a / b);
                    break;
                default:
                    fclose(file);
                    return error("Unknown operator.\n");
            }
        }
    }

    fclose(file);
    double result;
    if (pop(head, &result) != EXIT_SUCCESS)
        return 0;

    return result;
}
int freeAll(Position* head){
    Position* temp = NULL;

    while(head != NULL){
        temp = head;
        head= head->next;
        free(temp);
    }
    return EXIT_SUCCESS;
}