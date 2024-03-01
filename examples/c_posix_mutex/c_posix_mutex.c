#include <pthread.h>

struct ProtectedVariable{
    pthread_mutex_t mutex;
    int value;
};

struct ProtectedVariable x = {PTHREAD_MUTEX_INITIALIZER, 0};

int main(int argc, char** argv){
    if(pthread_mutex_lock(&x.mutex) != 0){
        perror("mutex_lock");
        exit(1);
    }

    x.value++;

    if(pthread_mutex_unlock(&x.mutex) != 0){
        perror("mutex_unlock");
        exit(1);
    }
}
