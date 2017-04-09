#include <iostream>
#include <future>
#include <vector>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <thread>
#include <functional>

template <class Value>
class thread_pool {
public:
    explicit thread_pool(size_t num_workers);
    std::future<Value> submit(std::function<Value()> function);
    void shutdown();
private:
    void processTasks();
    bool getStatusOfWork ();

    std::atomic<bool> isShutdown;
    std::condition_variable cv;
    std::vector<std::thread> threads;
    std::queue<std::function<Value()> > functionsQueue;
    std::queue<std::promise<Value> > promisesQueue;
    std::mutex mutex;
};

template <class Value>
thread_pool<Value>::thread_pool(size_t num_workers) :
    isShutdown(false)
{
    for (size_t i = 0;i < num_workers;++i){
        threads.emplace_back(&thread_pool<Value>::processTasks, this);
    }
}

template <class Value>
std::future<Value> thread_pool<Value>::submit(std::function<Value()> function)
{
    if (isShutdown.load()){
        throw std::runtime_error("Pool is shutdown\n");
    }
    std::unique_lock<std::mutex> lock(mutex);
    std::promise<Value> promise;
    std::future<Value> future = promise.get_future();
    functionsQueue.push(function);
    promisesQueue.push(std::move(promise));
    cv.notify_one();
    return future;
}

/***
 * "выключаем" пулл потоков, заканчиваем выполнение всех потоков
 */

template <class Value>
void thread_pool<Value>::shutdown()
{
    isShutdown.store (true);
    cv.notify_all();
    for (size_t i = 0;i < threads.size();++i){
        threads[i].join();
    }
}

/***
 * обработка всех получаемых заданий
 * по сути, каждый поток выполняет эту функцию
 */

template <class Value>
void thread_pool<Value>::processTasks()
{
    std::atomic<bool> isOK;
    isOK.store(true);
    while (isOK.load()){
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [this]() {
            return (!functionsQueue.empty() || isShutdown); //появилась задача или пулл выключили
        });
        if (!functionsQueue.empty()){
            std::promise<Value> promise = std::move (promisesQueue.front());
            promisesQueue.pop();
            std::function<Value()> function = functionsQueue.front();
            functionsQueue.pop();

            lock.unlock();
            try {
                promise.set_value(function());
            }
            catch(...){
                promise.set_exception(std::current_exception());
            }
        }
        else{
            isOK.store(false);
        }
    }
}
