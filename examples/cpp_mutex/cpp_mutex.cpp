#include <mutex>

class ConcurrentIndex {
    std::mutex mutex;       // protect the critical variable by 
                            // including it in a data structure with a mutex
    int index;

public:
    void increment(){
        mutex.lock();
        index++;            // danger: if this function would cause an exception
                            // or anything else that would exit this path of excecution
                            // the mutex wouldn't be unlocked
        mutex.unlock();
    }

    void decrement(){
        mutex.lock();
        index--;
        mutex.unlock();
    }

/*  If an object is created in a scope it's deleted when exiting the scope (in cpp)
 *  Thus wrapping the mutex in a object that handle the mutex will force it's unlocking
 *  (in it's constructor and destructor) 
 *  void increment() {
 *      const std::lock_guard<std::mutex> guard(mutex);
 *      index++;
 *  }
 */ 

/* For recursive mutex locking
 * use std::recursive_mutex it has counters for unlocking and won't deadlock
 */

};

