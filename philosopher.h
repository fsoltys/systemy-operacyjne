#include <condition_variable>
#include <vector>
#include <mutex>
#include <iostream>
#include <thread>

enum class State { THINKING, HUNGRY, EATING };  //enum states for readability

class philosopher {
private:
    int id, leftFork, rightFork;
    int thinkingTime, eatingTime;
    std::vector<std::mutex> &forksVecMTX;
    std::mutex &messageMTX;
    std::vector<State> &states;
    std::vector<std::condition_variable> &conditionVars;
    std::mutex &stateMutex;

    void takeForks();
    void putForks();
    void test(int philosopherId);

    void logState(State s);

public:
    philosopher(int id, int leftFork, int rightFork, std::vector<std::mutex> &forksVecMTX, std::mutex &messageMTX,
                std::vector<State> &states, std::vector<std::condition_variable> &conditionVars, std::mutex &stateMutex,
                int thinkingTime, int eatingTime);
    void run();
};


