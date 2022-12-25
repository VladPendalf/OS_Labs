#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char std[50];

pthread_mutex_t gMtx;

void* Writer(void* arg)
{
    size_t count = 0;
    while (1)
    {
        pthread_mutex_lock(&gMtx);
        snprintf(std,49,"Writer[%x]: count = %d", getpid(),count);
        pthread_mutex_unlock(&gMtx);
        count++;
        sleep(1);
    }
    pthread_exit(0);
}
  
void* Reader(void* arg)
{
    while (1)
    {
        sleep(1);
        pthread_mutex_lock(&gMtx);
        printf("\nReader[%x]: %s",(int)pthread_self(),std);
        pthread_mutex_unlock(&gMtx);
    }
    pthread_exit(0);
}

int main(void)
{
    pthread_mutex_init(&gMtx, NULL);
    pthread_t threads[11];

    int error;

    for (size_t i = 1; i < 11; i++)
    {
        error = pthread_create(&(threads[i]), NULL, &Reader, NULL);
        if (error != 0)
            printf("threads not creating\n");
    }

    pthread_create(&(threads[0]), NULL, &Writer, NULL);
    pthread_join( threads[0], NULL);
    for (size_t i = 1; i < 11; i++)
    {
        pthread_join( threads[i], NULL);
    }
    pthread_mutex_destroy(&gMtx);
  
    return 0;
}
