#include <thread>
#include <future>
#include <iostream>

void setter(std::promise<int>&& p){ // r-value reference, has no address
    p.set_value(32);
}

int main(){
    std::promise<int> retval;                       // create a promise obj
    std::future<int> future = retval.get_future();  // create a future interface obj
                                                    // that is linked to the promise
    std::thread thread(setter, std::move(retval));  // start a thread with setter
                                                    // and give the promise obj to it
                                                    // and remove it from main
    thread.join();                                  // join syncs, makes sure get won't wait      
    int value = future.get();

    std::cout << value << std::endl;  
}
