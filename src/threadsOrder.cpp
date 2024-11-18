#include <iostream>
#include <cstddef>
#include <thread>
#include <chrono>
#include <mutex>


static void doWork()
{
    std::mutex mt;
    std::cout << "========= doWork() started =========" << std::endl;
    for (size_t i = 0; i < 10; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        mt.lock();
        std::cout << "Thread id: " << std::this_thread::get_id()
            << " ========= doWork() working " << i << " =========" << std::endl;
        mt.unlock();
    }
    std::cout << "========= doWork() finished =========" << std::endl;
}

int main()
{
    std::cout << "========= main() started =========" << std::endl;
    
    // doWork() starts here, not after main()
    //
    std::thread t1(doWork);

    // t1.join();
    //
    // if this will be here, main() will not work, it will be waiting
    // for t1 end

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    
    std::mutex mt;
    for (size_t i = 0; i < 5; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        mt.lock();
        std::cout << "Thread id: " << std::this_thread::get_id()
            << " ========= main() working " << i << " =========" << std::endl;
        mt.unlock();
    }

    std::cout << "========= main() finished =========" << std::endl;
    
    // std::thread t1(doWork)
    //
    // if it will be here, main() will be done first, then
    // doWork() will be started
    
    t1.join(); // main() waits doWork() will be finished

    return 0;
}
