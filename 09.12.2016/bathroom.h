#include <mutex>
#include <condition_variable>
#include <atomic>

class bathroom
{
public:
    bathroom()
    {
        isMan.store(false);
        isWoman.store(false);
        peopleCount = 0;
    }
    void enter_male()
    {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [&] {return !isWoman.load();});
        isMan.store(true);
        peopleCount++;
    }
    void leave_male()
    {
        std::lock_guard<std::mutex> lock(mutex);
        --peopleCount;
        if (peopleCount == 0){
            isMan.store(false);
        }
        cv.notify_all();
    }
    void enter_female()
    {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [&] {return !isMan.load();});
        isWoman.store(true);
        peopleCount++;
    }
    void leave_female()
    {
        std::lock_guard<std::mutex> lock(mutex);
        --peopleCount;
        if (peopleCount == 0){
            isWoman.store(false);
        }
        cv.notify_all();
    }
private:
    std::atomic<bool> isMan;
    std::atomic<bool> isWoman;
    std::condition_variable cv;
    std::mutex mutex;
    std::size_t peopleCount;
};
