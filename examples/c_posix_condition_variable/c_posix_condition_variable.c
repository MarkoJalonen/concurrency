#include <pthread.h>

struct Reservation {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int reserved;
};

int acquire(struct Reservation *v){
    if(pthread_mutex_lock(&v->mutex) != 0){
        perror("mutex_lock");
        exit(1);
    }
    // must be while, because a thread may snatch the freed variable
    // after the wake up but before the reservation
    // so this thread must re check the condition to not break the logic
    // and the go back to sleep
    while(v->reserved != 0){ // replaces the wait condition!
        if(pthread_cond_wait(&v->cond, &v->mutex) != 0){
            perror("cond_wait");
            exit(1);
        }
    }

    v->reserved = 1;

    if(pthread_mutex_unlock(&v->mutex) != 0){
        perror("mutex_unlock");
        exit(1);
    }
}

void release(struct Reservation *v){
    if(pthread_mutex_lock(&v->mutex) != 0){
        perror("mutex_lock");
        exit(1);
    }
    if(v->reserved){
        if(pthread_cond_signal(&v->cond) != 0){
            perror("cond_signal");
            exit(1);
        }
    }

    v->reserved = 0;

    if(pthread_mutex_unlock(&v->mutex) != 0){
        perror("mutex_unlock");
        exit(1);
    }
}

struct Reservation v = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, 0};

int main(int argc, char** argv){
    acquire(&v);
    // do something
    release(&v);
}