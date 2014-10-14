#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>

#define CLK Clock::getInstance()

class Clock
{
public:
    static Clock* getInstance();
    virtual ~Clock();
protected:
private:
    // use Clock as singleton
    static Clock* clock;
    static bool instanceExists;
    Clock();

    std::chrono::time_point<std::chrono::system_clock> startPoint;
};

#endif // CLOCK_H
