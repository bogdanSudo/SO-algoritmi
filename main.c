#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PROCS_NUMBER 10
#define FILE_NAME "/Users/bogdanmica/Desktop/sem2.an1/ATP/SJF/file.txt"
typedef struct Process {
    int execTime;//durata unui proces
    int remainingTime; // initial = cu execTime;
    int finishingTime;
    int priority; // 4,3, 2 si 1 ordinea de executie
    char name[30];
}Process;

// Fisier: NUME DURATA PRIORITATE

int readProcs(Process **procs, int *nrProc){
    int ok = 0, exec;
    char nume[30];
    printf("Doresti sa mai adaugi procese ?\n");
    printf("Daca da apasa 1 , altfel apasa 0 = ");
    scanf("%d", &ok);

    while(ok){
        printf("Introdu nume proces = ");
        scanf("%s", nume);

        printf("Introdu durata proces = ");
        scanf("%d", &exec);

        procs[*nrProc] = malloc(sizeof(Process));
        if(!procs[*nrProc]){
            fprintf(stderr, "nu s-a alocat procesul %d\n", *nrProc);
            free(procs);
            return -1;
        }
        strcpy(procs[*nrProc]->name,nume);
        procs[*nrProc]->execTime = exec;
        procs[*nrProc]->remainingTime = exec;
        (*nrProc) ++;

        printf("Doresti sa mai adaugi procese ?\n");
        printf("Daca da apasa 1 , altfel apasa 0 = ");
        scanf("%d", &ok);
    }

}
void getProcs(Process **procs, int nrProc){
    for(int i = 0; i < nrProc; i++){
        printf("Procesul %s are durata de %d secunde si se termina dupa %d secunde\n", procs[i]->name, procs[i]->execTime, procs[i]->finishingTime);
    }
}

int getAvgExecTime(Process **procs, int nrProcs){
    int avg = 0;
    for(int i = 0; i < nrProcs; i++){
        avg += procs[i]->finishingTime;
    }
    return avg / nrProcs;
}

void getFinishingTime(Process **procs, int nrProcs){
    procs[0]->finishingTime = procs[0]->execTime;
    for(int i = 1; i < nrProcs; i++){
        procs[i]->finishingTime = procs[i-1]->finishingTime + procs[i]->execTime;
    }
}

void sortSJF(Process **procs, int nrProcs){
    int i,j;
    Process *aux;
    //sortare clasica
    for(i = 0; i < nrProcs - 1; i++){
        for(j = i+1; j < nrProcs; j++){
            if(procs[i]->execTime > procs[j]->execTime){
                aux = procs[i];
                procs[i] = procs[j];
                procs[j] = aux;
            }
        }
    }

}


void getProcsRR(Process **procs, int nrProcs){
    for(int i = 0; i < nrProcs; i++){
        printf("Procesul %s are durata de executie %d, mai are de executat %d si are prioritatea %d\n",
               procs[i]->name, procs[i]->execTime, procs[i]->remainingTime, procs[i]->priority);
    }
}
int sortRR(Process **procs, int nrProcs){
    int i,j;
    Process *aux;
    for(i = 0; i < nrProcs-1; i++){
        for(j = i+1; j < nrProcs; j++){
            if(procs[i]->priority < procs[j]->priority){
                aux = procs[i];
                procs[i] = procs[j];
                procs[j] = aux;
            }
        }
    }
    return 0;
}

/*
 * verifica daca mai exista procese cu prioritatea curenta
 */
int checkCurrentPriority(int currentPriority, Process **procs, int nrProcs){
    for(int i = 0; i < nrProcs; i++){
        if(currentPriority == procs[i]->priority)
            return 1;
    }

    return 0;
}
void roundRobin(Process **procs, int nrProcs, int quanta){
    int remainingProcesses = nrProcs, clock = 0,priorityCurrentProc = procs[0]->priority;
    while (priorityCurrentProc > 0){

        while(checkCurrentPriority(priorityCurrentProc, procs, nrProcs)){
            for(int i = 0; i < nrProcs; i++){
                if(procs[i]->priority == priorityCurrentProc && procs[i]->remainingTime > 0){
                    procs[i]->remainingTime -= quanta;

                    if(procs[i]->remainingTime <= 0)
                        procs[i]->priority = 0;
                    printf("Procesul %s s-a executat %d si mai are de executat %d\n",
                           procs[i]->name, quanta, procs[i]->remainingTime);

                    clock += quanta;
                }
            }
        }
        priorityCurrentProc--;
    }
}

int main() {
    Process **procs;
    int nrProcs = 0, quanta, whichAlgorithm = 0;

    procs = malloc(PROCS_NUMBER * sizeof(Process*));
    if(!procs){
        fprintf(stderr, "nu s-a alocat vectorul procs\n");
        return -1;
    }

    readProcs(procs, &nrProcs);

    printf("Daca doresti sa rulezi FCSFS apasa tasta 1\n"
           "Daca doresti sa rulezi SJF apasa tasta 2\n"
           "Daca doresti sa rulezi RR apasa tasta 3\n");
    printf("Introdu numar algoritm = ");
    scanf("%d", &whichAlgorithm);
    if(whichAlgorithm == 1) {
        printf("Ordinea proceselor in FCFS\n");
        getFinishingTime(procs, nrProcs);
        getProcs(procs, nrProcs);
    }
    else if (whichAlgorithm == 2) {
        printf("Ordinea proceselor in SJF\n");
        sortSJF(procs, nrProcs);
        getFinishingTime(procs, nrProcs);
        getProcs(procs, nrProcs);
    }
    else if(whichAlgorithm == 3) {
        printf("Ordinea proceselor in Round-Robin\n");
        printf("Introduceti valoarea pentru quanta = ");
        scanf("%d", &quanta);
        for (int i = 0; i < nrProcs; i++) {
            printf("Introduceti clasa de prioritate pentru procesul %s = ", procs[i]->name);
            scanf("%d", &procs[i]->priority);
        }
        //sortam procesele dupa prioritate: cele cu prioritate cea mai mare sunt primele
        roundRobin(procs, nrProcs, quanta);
    }

    for(int i = 0; i < nrProcs; i++){
        free(procs[i]);
    }
    free(procs);
    return 0;
}
