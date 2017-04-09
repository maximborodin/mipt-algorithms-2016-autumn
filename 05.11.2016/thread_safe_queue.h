#include <iostream>
#include <vector>
#include <deque>
#include <condition_variable>
#include <atomic>

template <class Value, class Container = std::deque<Value>>
class thread_safe_queue{
public:
    explicit thread_safe_queue(size_t capacity) : capacity(capacity)
    {
        isShutdown.store(false);
    }
    void enqueue(Value v)
    {
        std::unique_lock<std::mutex> lock(mutex);
        enqueueCV.wait(lock, [this]() {
            return (isShutdown.load() || (container.size() < capacity));
        });
        if (isShutdown.load()){
            throw std::runtime_error("Queue is shutdown\n");
        }
        container.push_back(std::move(v));
        popCV.notify_one();
        lock.unlock();
    }
    void pop(Value& v)
    {
        std::unique_lock<std::mutex> lock(mutex);
        popCV.wait(lock, [this]() {
            return (isShutdown || !container.empty());
        });
        if (isShutdown.load() && container.empty()){
            throw std::runtime_error("Poping from empty queue after shutdown\n");
        }
        v = std::move(container.front());
        container.pop_front();
        enqueueCV.notify_one();
        lock.unlock();
    }
    void shutdown()
    {
        isShutdown.store(true);
        enqueueCV.notify_all();
        popCV.notify_all();
    }
private:
    std::size_t capacity;
    Container container;

    std::condition_variable enqueueCV;
    std::condition_variable popCV;
    std::atomic<bool> isShutdown;
    std::mutex mutex;
};
