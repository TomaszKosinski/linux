#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void print_id (int id) {
    std::cout<<"thread "<<id<<" tries to lock the mutex\n";
    std::unique_lock<std::mutex> lck(mtx);
    std::cout<<"thread "<<id<<" locks the mutex\n";
    while (!ready) {
        std::cout<<"thread "<<id<<" waits\n";
        cv.wait(lck);
        std::cout<<"thread "<<id<<" notified\n";
    }
    std::cout << "thread " << id << "ends\n";
}

void go() {
    std::cout<<"go acquires the lock"<<std::endl;
    std::unique_lock<std::mutex> lck(mtx);
    //ready = true;
    std::cout<<"go calles notify"<<std::endl;
    cv.notify_all();
}

int main ()
{
    std::thread threads[10];
    // spawn 10 threads:
    for (int i=0; i<10; ++i) {
        threads[i] = std::thread(print_id,i);
    }

    std::cout << "10 threads ready to race...\n";
    go();

    for (auto& th : threads) {
        th.join();
    }

  return 0;
}