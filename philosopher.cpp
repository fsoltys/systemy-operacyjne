#include "philosopher.h"

philosopher::philosopher(int id, int leftFork, int rightFork,
                         std::vector<std::mutex> &forksVecMTX,
                         std::mutex &messageMTX,
                         std::vector<State> &states,
                         std::vector<std::condition_variable> &conditionVars,
                         std::mutex &stateMutex,
                         int thinkingTime, int eatingTime)
    : id(id),
      leftFork(leftFork),
      rightFork(rightFork),
      thinkingTime(thinkingTime),
      eatingTime(eatingTime),
      forksVecMTX(forksVecMTX),
      messageMTX(messageMTX),
      states(states),
      conditionVars(conditionVars),
      stateMutex(stateMutex) {}


void philosopher::run() {   //main infinite loop, representing philosopher's life
    while (true) {
        logState(State::THINKING);
        std::this_thread::sleep_for(std::chrono::milliseconds(thinkingTime));

        takeForks();
        logState(State::EATING);
        std::this_thread::sleep_for(std::chrono::milliseconds(eatingTime));
        putForks();
    }
}


void philosopher::takeForks() { //philosopher is trying to take forks
    std::unique_lock<std::mutex> lock(stateMutex);  //locking
    states[id] = State::HUNGRY;
    logState(State::HUNGRY);
    test(id);
    while (states[id] != State::EATING) {
        conditionVars[id].wait(lock);
    }
}

void philosopher::putForks() {
    std::unique_lock<std::mutex> lock(stateMutex);
    states[id] = State::THINKING;
    logState(State::THINKING);
    test((id + 1) % states.size());
    test((id - 1 + states.size()) % states.size());
}

void philosopher::test(int philosopherId) {
    if (states[philosopherId] == State::HUNGRY &&
        states[(philosopherId + 1) % states.size()] != State::EATING &&
        states[(philosopherId - 1 + states.size()) % states.size()] != State::EATING) {
        states[philosopherId] = State::EATING;
        logState(State::EATING);
        conditionVars[philosopherId].notify_one();
    }
}

void philosopher::logState(State s) {
    std::string action = (s == State::THINKING) ? "thinking" :
                         (s == State::HUNGRY)   ? "hungry" :
                         "eating";

    std::lock_guard<std::mutex> lock(messageMTX);
    std::cout << "Philosopher " << id << " is " << action << std::endl;
}