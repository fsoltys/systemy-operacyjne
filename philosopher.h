#include <condition_variable>
#include <vector>
#include <mutex>
#include <iostream>

enum class State { THINKING, HUNGRY, EATING };  //enum states for readability

class philosopher {
private:
    int id;
    int thinkingTime, eatingTime;
    std::mutex &messageMTX;
    std::vector<State> &states;
    std::vector<std::condition_variable> &conditionVars;
    std::mutex &stateMutex;

    void takeForks();
    void putForks();
    void test(int philosopherId);

    void logState(State s);

public:
    philosopher(int id, std::mutex &messageMTX,
                std::vector<State> &states, std::vector<std::condition_variable> &conditionVars, std::mutex &stateMutex,
                int thinkingTime, int eatingTime);
    void run();
};


