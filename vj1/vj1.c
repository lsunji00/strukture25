#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int count(const char *file); //deklaririam funkciju

struct Student{
    char name[40];
    char surname[40];
    int pts;
};

struct Student* allocate(int numOfStud); // prototip funkcije

int load(struct Student *students, int n, const char *fileName); //za ucitavanje zapisa

int maxPts(struct Student *students, int n); //racunanje maksimalnog broja bodova

int print(struct Student *students, int n, int max); // ispis na ekran

int main(){

    const char *fileName="studenti.txt"; //pokazivac na datoteku 

    int numOfStud=count(fileName); //poziv na funkciju
    if(numOfStud<=0){
        printf("Datoteka je prazna ili se ne moze otvoriti!");
        return 1; //prekid programa u slucaju da funkcija vrati nulu
    }
    
    printf("Broj studenata: %d\n", numOfStud); //ispis

    struct Student* students=allocate(numOfStud); //poziv funkcije za dinamicku alokaciju
    if(students==NULL){
        return 1; //provjera alokacije
    }

    printf("Alocirana je memorija za %d studenata.\n",numOfStud);

    int loaded=load(students, numOfStud,fileName); //ucitavanje podataka iz datoteke

    if (loaded != numOfStud){
        printf("Učitano je %d od %d zapisa.\n", loaded, numOfStud);
    }
    else{
        printf("Ucitano je %d zapisa.\n",loaded);
    }

    int max=maxPts(students, numOfStud); 
    printf("\nNajveci broj bodova: %d\n", max); //ispis najveceg broja bodova
    print(students, numOfStud, max); //izvrsavanje ispisa

    free(students); //oslobadanje zauzete memorije

    return 0;
}

int count(const char *file){
    FILE *filePointer=fopen(file,"r");//otvaranje datoteke samo za citanje
    
    if (!filePointer){
        return 0; //ako se ne otvara datoteka funkcija vraca nulu
    }

    int counter=0;
    char buffer[256];

    while(fgets(buffer, sizeof(buffer), filePointer)!=NULL){ //fgets cita redak po redak cijelu datoteku
        counter ++; //brojac povecamo za jedan svaki procitani redak
    }
    fclose(filePointer); //zatvaram datoteku
    return counter; //vracam ukupan broj redaka u glavnu funkciju
}

struct Student* allocate(int numOfStud){
    struct Student* students=(struct Student*) malloc(numOfStud * sizeof(struct Student)); // malloc za zauzimanje memorije
    
    if (students==NULL){
        printf("Greska pri alokaciji!");
        return NULL;
    }

    return students;
}

int load(struct Student *students, int n, const char *fileName){
    FILE *fp = fopen(fileName, "r"); //otvaram datoteku za citanje
    if(!fp){
        return 0; //ako se ne otvori
    }

    int i=0;

    while(i < n && fscanf(fp, "%s %s %d",students[i].name,
    students[i].surname, &students[i].pts)==3){
        i++; //ako je broj zapisa jednak 3 onda idemo u novi redak
    }
    //nakon sto prode sve redke datoteku zatvaramo
    fclose(fp);
    return i; //vracam koliko je redaka ucitano
}

int maxPts(struct Student *students, int n){
    int max = students[0].pts;

    for(int i =1 ; i<n; i++){
        if(students[i].pts>max){
            max=students[i].pts;
        }
    }

    return max; //vraca najvece ostvarene bodove
}

int print(struct Student *students, int n, int max){
    printf("\nIme              Prezime            Bodovi             Relativno\n");
    printf("-----------------------------------------------------------------------\n");

    for(int i=0;i<n;i++){
        double relative=((double)students[i].pts/max)*100; //zadana formula
        printf("%s             %s             %d                %.2f\n",
        students[i].name,
        students[i].surname,
        students[i].pts,
        relative);
    }
    return 0;
}