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

    int shmid = shmget(key,1024,0666|IPC_CREAT| S_IRUSR | S_IWUSR | IPC_EXCL);

    //=====================================
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char* str_time = asctime(timeinfo);
    int   new_size = strlen(str_time)+9;
    //=====================================
    while (1)
    {
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        char *str = (char*) shmat(shmid, NULL, 0);
        str_time = asctime(timeinfo);
        snprintf(str, new_size, "[%x]: %s", getpid(), str_time);
//        printf(  "\nData writen: %s\n",str);
        shmdt(str);
        sleep(1);
   }

    return 0;
}
