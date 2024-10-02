#pragma once
#include <iostream>
#include <chrono>
using namespace std;

class Timer {
private:
    bool running = false;
    chrono::steady_clock::time_point start_time;
    chrono::steady_clock::time_point start_pause;
    chrono::steady_clock::time_point end_pause;
    float elapsed_paused_time;
    float time_passed;
public:
    void start() {
        start_time = chrono::steady_clock::now();
        running = true;
        elapsed_paused_time = 0.f;
        time_passed = 0.f;
    }

    void stop() {
        running = false;
        start_pause = chrono::steady_clock::now();
    }

    void reset() {
        running = false;
        time_passed = 0.f;
        elapsed_paused_time = 0.f;
        start_time = chrono::steady_clock::now();
    }

    void start_again() {
        running = true;
        end_pause = chrono::steady_clock::now();
        auto pause_duration = std::chrono::duration_cast<std::chrono::seconds>(end_pause - start_pause);
        elapsed_paused_time += pause_duration.count();
    }

    float getTime() {
        if (running) {
            auto end_time = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
            time_passed = duration.count() - elapsed_paused_time;
        }

        return time_passed;
    }

    bool getRunning() {
        return running;
    }
};