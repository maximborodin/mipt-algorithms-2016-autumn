#include <mutex>
#include <condition_variable>
#include <atomic>

class barrier {
public:
    explicit barrier(size_t num_threads);
    void enter();
private:
    size_t threadsCount;
    std::atomic<size_t> waitingThreads;
    std::condition_variable cv;
    std::mutex mutex;
};

barrier::barrier (size_t num_threads)
{
    threadsCount = num_threads;
    waitingThreads.store(0);
}

void barrier::enter()
{
    std::unique_lock<std::mutex> lock(mutex);
    if (waitingThreads++ >= threadsCount - 1){
        waitingThreads.store(0);
        cv.notify_all();
    }
    else{
        cv.wait(lock);
    }
}
