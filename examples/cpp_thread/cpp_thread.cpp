#include <thread>
#include <iostream>
#include <vector>

void hello(unsigned num){                               // something to make concurrent
    std::cout << "My thread # is " << num 
              << " (id: " << std::this_thread::get_id() // how to identify a thread obj.
              << ")" << std::endl;    
}

int main(){
    std::vector<std::thread> thread_buffer;             // thread object buffer
    for(auto i = 0; i < 32; i++){
        thread_buffer.push_back(std::thread(hello, i)); // function pointer and id to
    }                                                   // create a thread obj.
                                                        // no quarantee of excecution
                                                        // order or no splitting 
                                                        // (e.g. 218 print is possible
    for(auto& thread : thread_buffer){
        thread.join();                                  // close the thread obj.
    }
    return EXIT_SUCCESS;
}