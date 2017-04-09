#include <algorithm>
#include <future>
#include <queue>

// =============THREAD_POOL=========================

template <class Value>
class thread_pool {
public:
    explicit thread_pool(size_t num_workers);
    std::future<Value> submit(std::function<Value()> function);
    void shutdown();
    void wait(std::future<Value>&& future);
private:
    void processTasks();
    void performTask();

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
            std::function<Value()> function = std::move(functionsQueue.front());
            functionsQueue.pop();

            lock.unlock();
            function();
            try {
                promise.set_value();
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

template <class Value>
void thread_pool<Value>::wait (std::future<Value>&& future)
{
    while (future.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
        performTask();
    }
}

template <class Value>
void thread_pool<Value>::performTask()
{
    std::unique_lock<std::mutex> lock(mutex);
    if (!functionsQueue.empty()) {
        std::promise<Value> promise = std::move (promisesQueue.front());
        promisesQueue.pop();
        std::function<Value()> function = std::move(functionsQueue.front());
        functionsQueue.pop();
        lock.unlock();
        function();
        try {
            promise.set_value();
        }
        catch(...){
            promise.set_exception(std::current_exception());
        }
    }

}


// ===================PARALLEL_SORTER==========================

class parallel_sort
{
public:
    parallel_sort(size_t workers);

    template<typename RandomAccessIterator, typename Compare>
    void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp);
private:
    template<typename RandomAccessIterator, typename Compare>
    void mergeSort(RandomAccessIterator first, RandomAccessIterator last, Compare comp);

    thread_pool<void>* pool;
};


parallel_sort::parallel_sort(size_t workers)
{
    pool = new thread_pool<void>(workers);
}

template<typename RandomAccessIterator, typename Compare>
void parallel_sort::sort(RandomAccessIterator first,
                         RandomAccessIterator last, Compare comp)
{
    std::future<void> future = pool->submit([&]()
                                            {
                                                mergeSort(first, last, comp);
                                            });
    future.get();
    pool->shutdown();
};

template<typename RandomAccessIterator, typename Compare>
void parallel_sort::mergeSort(RandomAccessIterator first,
                              RandomAccessIterator last, Compare comp)
{
    if (last - first < 100) {
        std::sort(first, last, comp);
        return;
    }
    RandomAccessIterator middle = first + (last - first) / 2;
    auto left_part = [&]() -> void {
        mergeSort(first, middle, comp);
    };
    auto right_part = [&]() -> void {
        mergeSort(middle, last, comp);
    };
    std::future<void> future_left = pool->submit(left_part);

    pool->wait(std::move(future_left));
    right_part();

    std::inplace_merge(first, middle, last, comp);
};
