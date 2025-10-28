#define _CRT_SECURE_NO_WARNINGS
#define ERROR -1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person{
    char firstName[50];
    char lastName[50];
    int birthYear;
    struct Person* next;
} Person;

int error(const char* msg);

Person* allocate(const char* firstName, const char* lastName, int birthYear);

int addToStart(Person* head, const char* firstName, const char* lastName, int birthYear);
 
int printList(Person* head);

int addToEnd(Person* head, const char* firstName, const char* lastName, int birthYear);

Person* findByLastName(Person* head, const char* lastName);

int printByLastName(Person* head, const char* lastName);

int deleteByLastName(Person* head, const char* lastName);

int freeAll(Person* first);

int addAfter(Person* head, const char* targetLastName, const char* firstName, const char* lastName, int birthYear);

int addBefore(Person* head, const char* targetLastName, const char* firstName, const char* lastName, int birthYear);

int sortByLastName(Person* head);

int saveToFile(Person* head, const char* filename);

int loadFromFile(Person* head, const char* filename);

int main(){

    Person* head = (Person*)malloc(sizeof(Person));
    if (!head)
         return error("Neuspjela alokacija head elementa.");
    head->next = NULL;

    setbuf(stdout, NULL); // gasi buffer tako da se printf odmah prikazuje


    
    int choice;
    char firstName[50], lastName[50], targetLastName[50];
    int birthYear;

    while (1) {
        system("clear");
        printf("\n----- IZBORNIK -----\n");
        printf("1. Dodaj osobu na pocetak\n");
        printf("2. Dodaj osobu na kraj\n");
        printf("3. Ispisi listu\n");
        printf("4. Pronadi osobu po prezimenu\n");
        printf("5. Obrisi osobu po prezimenu\n");
        printf("6. Dodaj osobu nakon druge osobe\n");
        printf("7. Dodaj osobu ispred druge osobe\n");
        printf("8. Sortiraj listu po prezimenima\n");
        printf("9. Spremi listu u datoteku\n");
        printf("10. Ucitaj listu iz datoteke\n");
        printf("11. Oslobodi memoriju i izadi\n");
        printf("--------------------\n");
        printf("Odaberi opciju: ");
        scanf("%d", &choice);
        system("clear");
        switch (choice) {
            case 1:
                printf("Unesi ime, prezime i godinu rodenja: ");
                scanf("%s %s %d", firstName, lastName, &birthYear);
                addToStart(head, firstName, lastName, birthYear);
                break;
            case 2:
                printf("Unesi ime, prezime i godinu rodenja: ");
                scanf("%s %s %d", firstName, lastName, &birthYear);
                addToEnd(head, firstName, lastName, birthYear);
                break;
            case 3:
                printList(head);
                while (getchar() != '\n'); 
                getchar();
                break;
            case 4:
                printf("Unesi prezime: ");
                scanf("%s", lastName);
                printByLastName(head, lastName);
                break;
            case 5:
                printf("Unesi prezime osobe za brisanje: ");
                scanf("%s", lastName);
                deleteByLastName(head, lastName);
                while (getchar() != '\n'); 
                getchar(); 
                break;
            case 6:
                printf("Unesi prezime osobe iza koje zelis dodati novu osobu: ");
                scanf("%s", targetLastName);
                printf("Unesi ime, prezime i godinu rodenja nove osobe: ");
                scanf("%s %s %d", firstName, lastName, &birthYear);
                addAfter(head, targetLastName, firstName, lastName, birthYear);
                break;
            case 7:
                printf("Unesi prezime osobe ispred koje zelis dodati novu osobu: ");
                scanf("%s", targetLastName);
                printf("Unesi ime, prezime i godinu rodenja nove osobe: ");
                scanf("%s %s %d", firstName, lastName, &birthYear);
                addBefore(head, targetLastName, firstName, lastName, birthYear);
                break;
            case 8:
                sortByLastName(head);
                while (getchar() != '\n'); 
                getchar(); 
                break;
            case 9:
                saveToFile(head, "osobe.txt");
                while (getchar() != '\n'); 
                getchar(); 
                break;
            case 10:
                loadFromFile(head, "osobe.txt");
                while (getchar() != '\n'); 
                getchar(); 
                break;
            case 11:
                freeAll(head);
                printf("Sva memorija je oslobodena.\n");
                printf("Program zavrsen.\n");
                while (getchar() != '\n'); 
                getchar(); 
                return 0;
            default:
                printf("Nepostojeca opcija! Pokusaj ponovno.\n");
                break;
        }               
    }

    return 0;
}

int error(const char* msg){
    printf("%s\n",msg);
    return ERROR;
}
Person* allocate(const char* firstName, const char* lastName, int birthYear){
    Person* newPerson=(Person*)malloc(sizeof(Person));
    if(!newPerson){
        printf("Neuspjela alokacija memorije.\n");
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
    if(!newPerson){
        return error("Greska pri alociranju u funkciji addToStart.");
    }
    newPerson->next=head->next;
    head->next=newPerson;
    return 0;
}

int printList(Person* head){

    if(head->next==NULL){
        return error("Lista je prazna");
    }

    Person* current=head->next;
    while(current!=NULL){
        printf("Ime: %-10s  Prezime: %-10s  Godina rodenja: %d\n",  //while petlja se vrti dok ne dode do vrijednosti NULL
                current->firstName,
                current->lastName,
                current->birthYear);
        current=current->next;
    }
    
    return 0;
}
int addToEnd(Person* head, const char* firstName, const char* lastName, int birthYear){
    Person* newPerson=allocate(firstName, lastName, birthYear);
    if(!newPerson){
        return error("Greska pri alociranju u funkciji addToEnd.");
    }
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
        return error("Nema tog prezimena u listi.");
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
        return error("Nema tog prezimena u listi.");
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
int freeAll(Person* first){
    Person* current = first;
    Person* temp = NULL;

    while(current != NULL){
        temp = current;
        current = current->next;
        free(temp);
    }
    return 0;
}

int addAfter(Person* head, const char*targetLastName, const char* firstName, const char* lastName, int birthYear){
    Person* newPerson=allocate(firstName, lastName, birthYear);
    if(!newPerson){
        return error("Neuspjela alokacija u funkciji addAfter");
    }
    Person* target=findByLastName(head, targetLastName);
    if(!target){
        return error("Greska pri pronalasku clana s pravim prezimenom u funkciji addAfter.");
    }
    newPerson->next=target->next;
    target->next=newPerson;
    return 0;
}
int addBefore(Person* head, const char*targetLastName, const char* firstName, const char* lastName, int birthYear){
    Person* newPerson=allocate(firstName, lastName, birthYear);
    if(!newPerson){
        return error("Neuspjela alokacija u funkciji addBefore.");
    }
    Person* current=head;
    Person* target=findByLastName(head, targetLastName);
    if(!target){
        return error("Greska pri pronalasku clana s pravim prezimenom u funkciji addBefore.");
    }
    while(current->next!=target){
        current=current->next;
    }
    newPerson->next=current->next;
    current->next=newPerson;
    return 0;
}
int sortByLastName(Person* head) {
    if (head->next == NULL || head->next->next == NULL) {
        return error("Nema dovoljno elemenata za sortiranje.");
    }

    printf("\n------------------------------------- SORTIRANJE ------------------------------------\n\n");
    printf("            Prije sortiranja              |               Nakon sortiranja\n");
    printf("-------------------------------------------------------------------------------------\n");

    // 1️⃣ Stvaramo kopiju liste prije sortiranja
    Person* copyHead = (Person*)malloc(sizeof(Person)); //kopija liste u svrhu da ispisemo staru i novu(sortiranu) listu
    if (!copyHead) return error("Neuspjela alokacija memorije za kopiju liste.");
    copyHead->next = NULL;

    Person* current = head->next;
    while (current != NULL) {
        addToEnd(copyHead, current->firstName, current->lastName, current->birthYear); //ubacivanje liste prije sortiranja u kopiju liste
        current = current->next;
    }

    Person* prev;
    Person* curr;
    Person* temp = NULL;
    int swapped;

    do { //proces sortiranja liste
        swapped = 0;
        prev = head;
        curr = head->next;

        while (curr->next != temp) {
            if (strcmp(curr->lastName, curr->next->lastName) > 0) {
                Person* swap = curr->next;
                curr->next = swap->next;
                swap->next = curr;
                prev->next = swap;
                swapped = 1;
            } else {
                curr = curr->next;
            }
            prev = prev->next;
        }
        temp = curr;
    } while (swapped);

    Person* oldList = copyHead->next;
    Person* newList = head->next;

    while (oldList != NULL) { //ispis obe liste
        printf("%-20s %-15s %-4d | %-20s %-15s %-5d\n",
            oldList->firstName, oldList->lastName, oldList->birthYear,
            newList->firstName, newList->lastName, newList->birthYear
        );

        oldList = oldList->next;
        newList = newList->next;
    }

    printf("-------------------------------------------------------------------------------------\n");
    printf("Lista je uspjesno sortirana po prezimenima.\n");

    freeAll(copyHead); //oslobadanje memorije u kojoj je kopija stare(nesortirane) liste

    return 0;
}

int saveToFile(Person* head, const char* filename){
    if(head->next==NULL){
        return error("Lista je prazna, nema se sto spremiti.\n");
    }
    Person* current=head->next;

    FILE* file=fopen(filename,"w");
    if(!file){
        return error("Neuspjesno otvaranje datoteke.\n");
    }
    while(current!=NULL){
        fprintf(file, "%s %s %d\n",current->firstName,current->lastName,current->birthYear);
        current=current->next;
    }
    fclose(file);
    printf("Lista je uspjesno spremljena u datoteku.\n");
    return 0;
}
int loadFromFile(Person* head, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return error("Neuspjesno otvaranje datoteke za citanje.");
    }

    char firstName[50];
    char lastName[50];
    int birthYear;

    freeAll(head->next);
    head->next = NULL;

    while (fscanf(file, "%s %s %d", firstName, lastName, &birthYear) == 3) {
        addToEnd(head, firstName, lastName, birthYear);
    }

    fclose(file);
    printf("Lista je uspjesno ucitana iz datoteke '%s'.\n", filename);
    return 0;
}
