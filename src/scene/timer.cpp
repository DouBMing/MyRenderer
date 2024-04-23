#include "timer.h"
#include <iomanip>

Timer::Timer() : time(0), count(0) {}

void Timer::Start()
{
    start = std::chrono::high_resolution_clock::now();
    count++;
}

void Timer::Stop()
{
    auto end = std::chrono::high_resolution_clock::now();
    time += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

std::ostream& operator <<(std::ostream& os, Timer& timer)
{
    os << timer.count << "\t" << timer.time << "ms\t\t" << timer.time / timer.count << "ms";
    return os;
}

TimerManager* TimerManager::instance = nullptr;

TimerManager::TimerManager()
{
    if (instance != nullptr)
        delete instance;
    instance = this;
}

TimerManager::~TimerManager()
{
    for (auto t : timers)
        delete t.second;
}

void TimerManager::StartTimer(const std::string& name)
{
    auto it = instance->timers.find(name);
    if (it != instance->timers.end())
    {
        it->second->Start();
    }
    else
    {
        Timer* t = new Timer();
        t->Start();
        instance->timers.insert({name, t});
    }
}

void TimerManager::StopTimer(const std::string& name)
{
    auto it = instance->timers.find(name);
    if (it != instance->timers.end())
        it->second->Stop();
    else
        throw std::out_of_range("No such Timer");
}

void TimerManager::Print()
{
    std::cout << "name" << "\t\t" << "count" << "\t" << "total time" << "\t" << "average" << std::endl;
    for (const auto& t : instance->timers)
    {
        std::cout << std::setw(8) << std::left << t.first;
        std::cout << "\t" << *t.second << std::endl;
    }
}