#include <iostream>  // for std::cout
#include <queue>     // for the buffer data structure
#include <vector>    // to keep threads in
#include <semaphore> // to keep track of buffer status with counting_semaphore
#include <mutex>     // to protect buffer queue with a mutex
#include <thread>    // to use threaded processes
#include <random>    // to generate random numbers in the first process
#include "config.hh" // defines a divisor and a value to add

// number of elements in a buffer
#define N 10

/* A buffer class that keeps track of vector elements 
 * with two counting semaphores. The vector is protected
 * with a mutex. The mutex and semiphores are handled
 * by the member functions of the class.
 */

class Buffer
{
private:
    std::counting_semaphore<N> num_of_empty_elements{N};  // init to max elements 
    std::counting_semaphore<N> num_of_filled_elements{0}; // init to min elements
    std::mutex buffer_mutex;                              // to lock the queue
    std::queue<int> queue;                                // to store the elements
public:
    void push_element(int element)
    {
        num_of_empty_elements.acquire(); // decrease the empty count if possible
        {
            std::lock_guard<std::mutex> g(buffer_mutex); // if so, lock the vector
            queue.push(element);                         // and push the given element
        }
        num_of_filled_elements.release(); // increase the filled count
    }
    int pop_element(void)
    {
        int front;
        num_of_filled_elements.acquire(); // decrease the filled count if possible
        {
            std::lock_guard<std::mutex> g(buffer_mutex); // if so, lock the vector
            front = queue.front();                       // save an element
            queue.pop();                                 // and pop it
        }
        num_of_empty_elements.release(); // increase the empty count 
        return front;                    // return the saved element 
    }
};


void process_a(); // creates rand numbers
void process_b(); // divides them with CONFIG.DIV
void process_c(); // adds CONFIG.ADD to them
void process_d(); // prints them to cout

Buffer AB_buffer; // a buffer between process a and b
Buffer BC_buffer; // a buffer between process b and c
Buffer CD_buffer; // a buffer between process c and d

int main(void)
{
    std::vector<std::thread> thread_a_vector; // vector to hold the a threads
    std::vector<std::thread> thread_b_vector; // vector to hold the b threads
    std::vector<std::thread> thread_c_vector; // vector to hold the c threads
    std::vector<std::thread> thread_d_vector; // vector to hold the d threads

    uint64_t i; // for loop iterator for thread creation
    for(i = 0; i < 4; i++) // push four instances of 
    {
        thread_a_vector.push_back(std::thread(process_a)); // thread a 
        thread_b_vector.push_back(std::thread(process_b)); // thread b
        thread_c_vector.push_back(std::thread(process_c)); // thread c
        thread_d_vector.push_back(std::thread(process_d)); // thread d
    }

    // synchronize when threads are done, currently never
    for(i = 0; i < thread_a_vector.size(); i++) 
    {
        thread_a_vector[i].join();
        thread_b_vector[i].join();
        thread_c_vector[i].join();
        thread_d_vector[i].join();
    }
}

void process_a()
{
    // create an interface and a generator of random numbers
    std::random_device rd;
    std::mt19937 gen( rd() );
    std::uniform_int_distribution<int> dist( 0, 0x1000000 ); // create a range
    while(true)
    {
        AB_buffer.push_element( dist( gen ) ); // create a number with them and push it to the next buffer
    }
}

void process_b()
{
    while(true)
    {
        // get a value from the buffer before and divide it with DIV before pushin it to the next buffer
        BC_buffer.push_element( (int) ( (uint64_t) AB_buffer.pop_element() / CONFIG.DIV) );
    }
}

void process_c()
{
    while(true)
    {
        // get a value from the buffer before and sum it with ADD before pushin it to the next buffer
        CD_buffer.push_element( (int) ( (uint64_t) BC_buffer.pop_element() + CONFIG.ADD) );
    }
}

void process_d()
{
    while(true)
    {
        // // get a value from the buffer before and print it
        std::cout << CD_buffer.pop_element() << '\n';
    }
}