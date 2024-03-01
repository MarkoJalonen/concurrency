#include <thread>
#include <iostream>
#include <future>

// async is not anymore recommended! has fun edge cases

int calcA(){
    return 16;
}

int calcB(){
    return 8;
}

int main(){
    // async may not create a thread, 
    // main thread may just jump to the function
    // at .get
    std::future<int> resultA = std::async(calcA);
    std::future<int> resultB = std::async(calcB);
    // see future and promise 

    // get method returns AND joins, 
    // normally join won't return anything,
    // if the result is not ready: wait
    std::cout << resultA.get() + resultB.get() << std::endl;
    return 0;
}