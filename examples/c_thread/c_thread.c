#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *hello(void *arg){
    long int number = (long int)arg;
    if(pthread_detach(pthread_self()) != 0){ // detached from main to background
        perror("detach failed!");            // thus no need for join
        exit(1);
    }
    fprintf(stdout, "My thread # is: %d\n", number);
    return NULL;
}

int main(int argc, char **argv){
    int status;
    pthread_t thread;
    for(long int i = 0; i < 32; i++){
        status = pthread_create(&thread, NULL, &hello, (void*)i);
        if(status != 0){
            perror("pthread_create failed");
            exit(1);
        }
    }
    sleep(2);
    return 0;
}