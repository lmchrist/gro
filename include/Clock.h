#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <iostream>

#define CLK_start() Clock::getInstance()
#define CLK_printStartTime() Clock::getInstance()->printStartTime()
#define CLK_printTimeSinceStart() Clock::getInstance()->printTimeSinceStart()
#define CLK_millisecSinceEpoch() Clock::getInstance()->millisecSinceEpoch()

class Clock
{
public:
    static Clock* getInstance();
    virtual ~Clock();

    std::string printStartTime();
    std::string printTimeSinceStart();

    unsigned int millisecSinceEpoch();

protected:
private:
    // use Clock as singleton
    Clock();

    std::string convertMillisecToString(unsigned int);

    std::chrono::time_point<std::chrono::system_clock> clockStart;
};

#endif // CLOCK_H
