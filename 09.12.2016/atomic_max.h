#include <iostream>
#include <atomic>
#include <mutex>

int fetch_and_set_max(std::atomic<int>& x, int y)
{
    std::atomic<int> maxVal;
    maxVal.store(std::max(x.load(), y));
    return x.exchange(maxVal);
}