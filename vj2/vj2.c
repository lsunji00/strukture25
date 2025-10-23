#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person{
    char firstName[50];
    char lastName[50];
    int birthYear;
    struct Person* next;
} Person;

Person* allocate(const char* firstName, const char* lastName, int birthYear);

int addToStart(Person* head, const char* firstName, const char* lastName, int birthYear);
 
int printList(Person* head);

int addToEnd(Person* head, const char* firstName, const char* lastName, int birthYear);

Person* findByLastName(Person* head, const char* lastName);

int printByLastName(Person* head, const char* lastName);

int deleteByLastName(Person* head, const char* lastName);

int freeAll(Person* head);

int main(){

    Person* head=(Person*)malloc(sizeof(Person));
    head->next=NULL;
    
    addToStart(head, "Ivan", "Ivic", 2003);
    addToStart(head, "Luka", "Lukic", 2003);
    addToStart(head, "Marko", "Markic", 2005);
    addToEnd(head, "Marino", "Marijanovic", 2000);
    printList(head);
    printByLastName(head, "Lukic");
    deleteByLastName(head, "Marijanovic");
    printf("\n            Finalna lista:\n-----------------------------------------\n");
    printList(head);

    freeAll(head);


    
    

    return 0;
}

Person* allocate(const char* firstName, const char* lastName, int birthYear){
    Person* newPerson=(Person*)malloc(sizeof(Person));
    if(!newPerson){
        printf("Neuspjesna alokacija memorije.");
        return NULL;
    }

    strcpy(newPerson->firstName, firstName);
    strcpy(newPerson->lastName, lastName);
    newPerson->birthYear=birthYear;
    newPerson->next=NULL;

    return newPerson;
}

int addToStart(Person* head, const char* firstName, const char* lastName, int birthYear){ //funkcija koristi funkciju za alociranje te stavlja element liste na pocetak
    Person* newPerson= allocate(firstName, lastName, birthYear);
    newPerson->next=head->next;
    head->next=newPerson;
    return 0;
}

int printList(Person* head){

    if(head->next==NULL){
        printf("Lista je prazna.");
        return -1;
    }

    Person* current=head->next;
    while(current!=NULL){
        printf("Ime: %s  %s  Godina rodenja: %d\n",  //while petlja se vrti dok ne dode do vrijednosti NULL
                current->firstName,
                current->lastName,
                current->birthYear);
        current=current->next;
    }
    
    return 0;
}
int addToEnd(Person* head, const char* firstName, const char* lastName, int birthYear){
    Person* newPerson=allocate(firstName, lastName, birthYear);
    if(!newPerson) return -1;
    Person* current=head;
    while(current->next!=NULL){
        current=current->next;
    }
    current->next=newPerson;
    return 0;
}
Person* findByLastName(Person* head, const char* lastName){
    Person* current=head->next;
    while(current!=NULL){
        if(strcmp(current->lastName,lastName)==0){
            return current;
        }
        current=current->next;
    }
    return NULL;
}
int printByLastName(Person* head, const char* lastName){
    Person* current=findByLastName(head,lastName);
    if(current==NULL){
        printf("Nema tog prezimena u listi.");
        return 0;
    }
    printf("Pronaden je student s tim prezimenom\nIme: %s  %s  Godina rodenja: %d \n",
                current->firstName,
                current->lastName,
                current->birthYear);
    
    return 0;
}
int deleteByLastName(Person* head, const char* lastName){
    Person* current=findByLastName(head,lastName);
    if(current==NULL){
        printf("Nema tog prezimena u listi.");
        return 0;
    }
    Person* prev=head;
    while(prev->next!=current){
        prev=prev->next;
    }
    printf("Obrisan je student: %s  %s  Godina rodenja: %d\n",
            current->firstName,
            current->lastName,
            current->birthYear);
    prev->next=current->next;
    free(current);


    return 0;
}
int freeAll(Person* head){
    while(head->next!=NULL){
        Person* temp=head;
        head=head->next;
        free(temp);
    }
    printf("Sva memorija je oslobodena.");
    return 0;
}