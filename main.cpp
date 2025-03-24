#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include "philosopher.h"

int main(int argc, char* argv[]) {

    if (argc == 2 && std::string(argv[1]) == "--help") {
        std::cout << "Usage: " << argv[0] << " [num_philosophers] [thinking_time_ms] [eating_time_ms]\n"
                  << "Example:\n"
                  << "  " << argv[0] << " 5 1000 1500\n"
                  << "  (5 philosophers, 1000ms thinking, 1500ms eating)\n\n"
                  << "If no arguments are provided, defaults are used:\n"
                  << "  5 philosophers, 1000ms thinking, 1000ms eating\n";
        return 0;
    }

    //default values
    int thinkingTime = 1000;
    int eatingTime = 1000;
    int philosopherCount = 5;

    //initialization
    std::vector<std::mutex> forks(philosopherCount);
    std::vector<State> states(philosopherCount, State::THINKING);
    std::vector<std::condition_variable> condVars(philosopherCount);
    std::mutex stateMutex;
    std::mutex messageMutex;
    std::vector<std::thread> threads;
    std::vector<std::unique_ptr<philosopher>> philosophers;

    //parsing arguments
    try {
        if (argc >= 2) {
            philosopherCount = std::stoi(argv[1]);
            if (philosopherCount <= 2) {
                std::cerr << "Error: Number of philosophers must be > 2.\n";
                return 1;
            }
        }

        if (argc >= 3) {
            thinkingTime = std::stoi(argv[2]);
            if (thinkingTime <= 0) {
                std::cerr << "Error: Thinking time must be > 0.\n";
                return 1;
            }
        }

        if (argc >= 4) {
            eatingTime = std::stoi(argv[3]);
            if (eatingTime <= 0) {
                std::cerr << "Error: Eating time must be > 0.\n";
                return 1;
            }
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: All arguments must be integers.\n";
        return 1;
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: Argument value is out of range.\n";
        return 1;
    }


    for (int i = 0; i < philosopherCount; ++i) {
        philosophers.emplace_back(std::make_unique<philosopher>(
            i, i, (i + 1) % philosopherCount,
            forks, messageMutex, states, condVars, stateMutex,
            thinkingTime, eatingTime // przekazanie czasów
        ));
    }

    for (auto& phil : philosophers) {
        threads.emplace_back(&philosopher::run, phil.get());
    }

    for (auto& t : threads) {
        t.join(); // w praktyce: program się nie kończy
    }

    return 0;
}