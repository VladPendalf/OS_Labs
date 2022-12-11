#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <limits.h>
#include <string.h>

void My_atexit() {printf("Exit from\n", getpid());}

void Sig_Int(int num) {
    printf("getpid =  %i getppid = %i num = %i strsignal(num) = (%s)\n", 
            getpid(), getppid(), 
            num, strsignal(num)
        );
}

void Sig_Term(int num) {
    printf("getpid =  %i getppid = %i num = %i strsignal(num) = (%s)\n", 
            getpid(), getppid(),
            num, strsignal(num)
        );
}


main() 
{
    pid_t pid = fork();
    //================================================================
    switch(pid)
    {
        case -1:
            perror("Smth_Error");
            return 1;
        break;
        
        case 0:
            printf(" CHILD: Мой PID -- %d\n"           , getpid());
            printf(" CHILD: PID моего родителя -- %d\n", getppid());
        break;
        
        default:
            printf("PARENT: Мой PID -- %d\n"       ,     getpid());
            printf("PARENT: PID моего потомка %d\n",     getppid());
        break;
    }
    //================================================================
    if (atexit(My_atexit) != 0) //Upon successful completion, atexit() shall return 0; otherwise, it shall return a non-zero value.  
    {
        fprintf(stderr, "Not found function to exit!\n");
        return 1;
    }
    //================================================================
    if (signal(SIGINT, Sig_Int) == SIG_ERR) 
    {
        perror("signal() not working");
        return 1;
    }
    //================================================================
    struct sigaction act; 
    //https://habr.com/ru/post/141206/
    //https://unetway.com/tutorial/c-obrabotka-signalov
    act.sa_handler = Sig_Term;
    sigemptyset(&act.sa_mask);
    sigaction(SIGTERM, &act, NULL);
    //================================================================

    if (pid > 0) 
    {
        int Exit_Code = 0;
        pid_t waitId = wait(&Exit_Code);

        if (waitId < 0) return 1;

        printf("PARENT: Child die with code %i (%s)\n", 
                Exit_Code, strsignal(Exit_Code)
            );
        return 0;
    }

    while (1) {}

    return 0;
}
