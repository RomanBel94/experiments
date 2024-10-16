#include <cstddef>
#include <thread>
#include <chrono>
#include <iostream>


void doWork()
{
    std::cout << "========= doWork() started =========" << std::endl;
    for (size_t i = 0; i < 10; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "========= doWork() working " << i << " =========" << std::endl;
    }
    std::cout << "========= doWork() finished =========" << std::endl;
}

int main()
{
    std::cout << "========= main() started =========" << std::endl;
    
    // doWork() starts here, not after main()
    //
    std::thread t1(doWork);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    
    for (size_t i = 0; i < 5; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "========= main() working " << i << " =========" << std::endl;
    }

    std::cout << "========= main() finished =========" << std::endl;
    
    // std::thread t1(doWork)
    //
    // if it will be here, main() will be done first, then
    // doWork() will be started
    
    t1.join(); // main() waits doWork() will be finished

    return 0;
}
