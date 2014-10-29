#include "types.h"
#include "user.h"


int value = 1;

void sig_handled(void){

    printf(1,"Signal received \n");
    value = 0;
    printf(1,"value = %d\n",value);

}

void sig2(void){
    printf(1,"YOLOSWAG\n");
    value = 0;
    printf(1,"value = %d\n",value);
}

void sig3(void){
    printf(1,"BIEBERFIEVER\n");
    value = 0;
    printf(1,"value = %d\n",value);   
}

void sig4(void){
    printf(1,"Guys\n");
    value = 0;
    printf(1,"value = %d\n",value);   
}

int main(int argc, char *argv[]){

    int pid;
    int signum;

    if(argc <= 1){
           printf(2,"Error use test_signal signum \n");
       exit();
     }



     signum = atoi(argv[1]);
    //Creating another process

    pid = fork();
    switch(pid){
        case -1: //There was a problem
            printf(2,"Error running fork");
            break;

        case 0: //child
            printf(1,"I am the child: %d\n", getpid());
            printf(1,"Waiting for a signal \n");
            //Setup the signal and wait for it
            switch (signum){
                case 1:
                    signal(signum, sig_handled);
                    break;
                case 2:
                    signal(signum, sig2);
                    break;
                case 3:
                    signal(signum, sig3);
                    break;
                case 4:
                    signal(signum, sig4);
                    break;
                default:
                    signal(1,sig_handled);
                    break;
            }
            while(value){};
            printf(signum,"\nWhile completed\n");
            break;
        default://Parent
            //Wait for the child to start
            sleep(10);
            //Send the signal
            printf(1,"Sending signal %d to %d\n",signum,pid);
            killsignal(pid,signum);
            //Wait for the child to complete
            wait();
            printf(2,"Child completed\n");
    }
    exit();
}


