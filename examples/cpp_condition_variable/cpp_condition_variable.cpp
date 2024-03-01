#include <mutex>
#include <condition_variable>

class Reserve {
    private: 
        std::mutex lockvariable;
        std::condition_variable notice;
        bool reserved = false;
    public:
        void acquire() {
            // create a lock
            std::unique_lock<std::mutex> uniquelock(lockvariable);
            // link the lock and condition variable with a check function (lambda)
            // the thread goes to sleep and waits for sync from an another thread
            // instead of the mutex case where the thread locking needs to unlock it
            // when put to wait the mutex is freed by condition variable
            notice.wait(uniquelock, [this](){return reserved == false;});
            // after the wait checks out
            reserved = true;
        }
        void release() {
            // because the mutex was freed, an other thread may enter a critical part 
            std::unique_lock<std::mutex> uniquelock(lockvariable);
            if(reserved){
                // another thread can wake up the other thread
                // but does release the mutex
                notice.notify_one();
                // notify one selects only one sleeping thread sets it to ready
            }
            // so we can still do other stuff with our mutex
            reserved = false;
        }
        // in other words mutex only allow one thread to be in RUN state
        // inside the critical part, but there may be multiple threads in it
};