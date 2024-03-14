#include <semaphore.h>

struct ProtectedVariable{
    sem_t mutex; // semaphore will be initialized to 1, behaves like a mutex
    int value;
};

int main(int argc, int **argv){
    struct ProtectedVariable x;
    sem_t *mutex = &x.mutex; 
    
    sem_init(mutex, 0, 1);  // semaphore*, threads run in the same process,
                            // inital value

    sem_wait(mutex);    // reserve a token
    x.value++;          // protected operation
    sem_post(mutex);    // release the token

    sem_destroy(mutex); // release resources

    return 0;
}