#ifndef SEARCH_FOR_AT_TIME_TEST_UTILITIES_H
#define SEARCH_FOR_AT_TIME_TEST_UTILITIES_H

#include <chrono>

template<typename T>
concept TimerDuration = requires {
    typename std::chrono::nanoseconds;
    typename std::chrono::microseconds;
    typename std::chrono::milliseconds;
    typename std::chrono::seconds;
    typename std::chrono::minutes;
    typename std::chrono::hours;
};

template<TimerDuration T>
class Timer {
private:
    std::chrono::steady_clock::time_point _start_time;
    std::chrono::steady_clock::time_point _check_time;

public:
    Timer () {
        _start_time = std::chrono::steady_clock ::now();
        _check_time = _start_time;
    }

    void start () {
        _start_time = std::chrono::steady_clock ::now();
    }

    std::chrono::milliseconds next_lap () {
        auto next_time = std::chrono::steady_clock ::now();
        auto interval = std::chrono::duration_cast<T>(next_time - _check_time);
        _check_time = next_time;
        return interval;
    }

    std::chrono::milliseconds check () {
        return std::chrono::duration_cast<T>(_check_time - _start_time);
    }

    std::chrono::milliseconds stop () {
        _check_time = std::chrono::steady_clock ::now();
        return std::chrono::duration_cast<T>(_check_time - _start_time);
    }
};

#endif //SEARCH_FOR_AT_TIME_TEST_UTILITIES_H
