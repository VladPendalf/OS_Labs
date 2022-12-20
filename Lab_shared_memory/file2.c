#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
//==================TIME=============================
time_t      rawtime;
struct tm * timeinfo;
//===================================================

int main()
{
    pid_t pid;
    key_t key = ftok("shmfile",65);

    int shmid = shmget(key,1024,0666|IPC_CREAT);

    //=====================================
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char* str_time = asctime(timeinfo);
    //=====================================


    while (1)
    {
        time(&rawtime);
        timeinfo = localtime(&rawtime);
    
        str_time = asctime(timeinfo);

        char *str = (char*) shmat(shmid, NULL ,0);

        printf("\n%s",str);
        printf("\n[%d]: %s", getpid(), str_time);

        shmdt(str);
        sleep(1);
    }
        shmctl(shmid,IPC_RMID,NULL);
    

    return 0;
}
