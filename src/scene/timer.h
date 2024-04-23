#pragma once
#include <iostream>
#include <chrono>
#include <map>

class Timer
{
public:
    Timer();

    void Start();
    void Stop();

    friend std::ostream& operator <<(std::ostream& os, Timer& timer);
private:
    std::chrono::_V2::system_clock::time_point start;
    float time;
    int count;
};

std::ostream& operator <<(std::ostream& os, Timer& timer);

class TimerManager
{
public:
    TimerManager();
    ~TimerManager();

    static void StartTimer(const std::string& name);
    static void StopTimer(const std::string& name);
    static void Print();
private:
    static TimerManager* instance;
    std::map<std::string, Timer*> timers;
};