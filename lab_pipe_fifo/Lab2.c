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

//==================TIME=============================
time_t rawtime;
struct tm * timeinfo;
//===================================================

void writer(int fd) //parent
{
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    char* str = asctime(timeinfo);

    int new_size = strlen(str)+15;
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
    
    char* buf = calloc(sizeof(char),32);
    read(fd, buf, 32);

    printf("%s",buf);
    free(buf);
    printf("\n[%d]: ", getpid());
    printf("%s\n", str);
}


int main()
{
    //==================PIPE=============================
    int fd[2];
    pipe(fd);

    pid_t childpid = fork();
    switch (childpid)
    {
    case -1:
        printf("fork error\n\a");
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
    mkfifo("fifo.fifo", 0666);
    int id;

    childpid = fork();  
    switch (childpid) 
    {
    case -1:
        printf("Fork error");
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