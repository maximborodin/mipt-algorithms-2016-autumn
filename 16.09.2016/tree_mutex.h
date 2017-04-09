#include <cstdio>
#include <iostream>
#include <atomic>
#include <vector>
#include <array>
#include <thread>
#include <algorithm>
#include <exception>

class PetersonMutex
{
public:
    PetersonMutex()
    {
        want_[0].store(false);
        want_[1].store(false);
        victim_.store(0);
    }

    void lock(int t)
    {
        want_[t].store(true);
        victim_.store(t);
        while (want_[1 - t].load() && victim_.load() == t) {
            std::this_thread::yield();
        }
    }

    void unlock(int t) {
        want_[t].store(false);
    }

private:
    std::array<std::atomic<bool>, 2> want_;
    std::atomic<int> victim_;
};

class tree_mutex
{
public:
    tree_mutex (std::size_t num_threads);
    void lock (std::size_t thread_index);
    void unlock (std::size_t thread_index);
private:
    std::size_t calcTreeSize(std::size_t threadsCount);
    std::vector<PetersonMutex> mutexes;
};



std::size_t tree_mutex::calcTreeSize (std::size_t threadsCount)
{
    std::size_t result = 2;
    while (result < threadsCount){
        result *= 2;
    }
    result--;
    return result;
}

tree_mutex::tree_mutex (std::size_t num_threads) : mutexes(calcTreeSize(num_threads))
{
    /*std::size_t mutexesCount = calcTreeSize(num_threads);
    mutexes(mutexesCount, PetersonMutex());*/
}

void tree_mutex::lock(std::size_t thread_index)
{
    //mutexes[-2].lock(0);
    std::size_t curIndex = mutexes.size() + thread_index; // index of current mutex
    std::size_t nextIndex = (curIndex - 1) / 2;
    while (nextIndex != 0){
        if (curIndex % 2 == 1){
            mutexes[nextIndex].lock(0);
        }
        else{
            mutexes[nextIndex].lock(1);
        }
        curIndex = nextIndex;
        nextIndex = (nextIndex - 1) / 2;
    }
    if (curIndex % 2 == 1){
        mutexes[nextIndex].lock(0);
    }
    else{
        mutexes[nextIndex].lock(1);
    }
    //mutexes[-2].lock(0);
}

void tree_mutex::unlock(std::size_t thread_index)
{
    std::size_t curIndex = mutexes.size() + thread_index; // index of current mutex
    std::vector<std::pair<std::size_t, bool> > path; // first - index of mutex, second - 0 and 1 - which son has sent thread
    while (curIndex > 0){
        if (curIndex % 2 == 1){
            path.push_back(std::make_pair((curIndex - 1) / 2, 0));
        }
        else{
            path.push_back(std::make_pair((curIndex - 1) / 2, 1));
        }
        curIndex = (curIndex - 1) / 2;
    }
    std::reverse(path.begin(), path.end());
    for (size_t i = 0;i < path.size();i++){
        mutexes[path[i].first].unlock(path[i].second);
    }
}
