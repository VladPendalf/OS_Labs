#define _GNU_SOURCE

/// 2a) Создать pipe. 
/// Создать дочерний процесс. 
/// Передать строчку через pipe из родительского в дочерний процесс с текущим временем и pid передающего процесса. 
/// Дочерний процесс должен вывести на экран его текущее время и полученную через pipe строчку.
/// Время из родительского и дочернего должно различаться как минимум на 5 секунд.
// 2б) Аналогично 2а, но использовать fifo

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
/*====================================================/*
ECЛИ ЧТО-ТО ВЫГЛЯДИТ, КАК ГОВНО, ПАХНЕТ КАК ГОВНО И НА
ВКУС ГОВНО, ТО НАВЕРНОЕ - ЭТО ГОВНО
//====================================================*/

//==================TIME=============================
time_t rawtime;
struct tm * timeinfo;
//===================================================

void writer(int fd) //parent
{
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    char* str = asctime(timeinfo);

    int new_size = strlen(str)+8;
    char result[new_size];

    snprintf(result, new_size, "[%d]: %s", getpid(), str);

    write(fd, result, strlen(result));
}

void reader(int fd)  //child
{
    sleep(5);
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    char* str = asctime(timeinfo);
    
    char buf = 0;
    while (read(fd, &buf, 1) > 0) 
        printf("%c", buf);

    printf("\n[%d]: ", getpid());
    printf("%s\n", str);
}


int main()
{
    //==================PIPE=============================
    int fd[2];
    if(pipe(fd) != 0)
    {
        perror("Pepe smth error\n\a");
        exit(EXIT_FAILURE);
    }

    pid_t childpid = fork();
    switch (childpid)
    {
    case -1:
        perror("fork error\n\a");
        exit(EXIT_FAILURE);
        break;

    case 0:
        close(fd[1]); 
        reader(fd[0]);
        close(fd[0]);
        break;
    
    default:
        close(fd[0]); 
        writer(fd[1]);
        close(fd[1]);
        break;
    }

    wait(NULL);

    //===================FIFO============================
    printf("i'm sleeping before FIFO is started\n");
    sleep(5);
    int fifo = mkfifo("fifo.fifo", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    int id;
    if (fifo < 0) 
    {
        perror("FIFO error");
        exit(EXIT_FAILURE);
    }

    childpid = fork();  
    switch (childpid) 
    {
    case -1:
        perror("Fork error");
        exit(EXIT_FAILURE);
        break;

    case 0:
        id = open("fifo.fifo", O_RDONLY);
        reader(id);
        close(id);
        break;

    default:
        id = open("fifo.fifo", O_WRONLY);
        writer(id);
        close(id);
        break;
    }
    remove("fifo.fifo");
    wait(NULL);
    kill(childpid,SIGTERM);
    //===============================================
    
    return 0;
}