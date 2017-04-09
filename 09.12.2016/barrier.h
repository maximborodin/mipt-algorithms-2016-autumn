#include <atomic>
#include <condition_variable>
#include "semaphore.h"
#include <mutex>

class barrier {
public:
    explicit barrier() : sem0(0), in(1), mainSem(1), out(0){
        number = 2;
    }

    void enter(){
        in.aquire();//семафор для входящих мьютексов
        if (number == 0){
            number = 2;
        }
        --number;//оставшееся количество

        if (number == 0){
            counte = 0;
            in.release();
            sem0.release();
        }
        else{
            in.release();
            sem0.aquire();
        }
        sem0.release();
        sem0.aquire();
        mainSem.aquire();//решает, кто выйдет

        ++counte;
        if (counte == 1){
            mainSem.release();
            out.aquire();
        }
        else{
            mainSem.release();
            out.release();
        }
    }

private:
    std::atomic<int> number, counte;
    semaphore sem0, in, mainSem, out;
};
