#include "Clock.h"

Clock::Clock()
{
    timestamp_start = millisecSinceEpoch();
}

Clock::~Clock()
{
}

Clock* Clock::getInstance()
{
    // put singleton on heap
    static Clock* instance = new Clock();
    return instance;

}

std::string Clock::printStartTime()
{
    std::cout << timestamp_start << std::endl;

    return "TODO";
    //return convertMillisecToString( timestamp_start );
}

std::string Clock::printTimeSinceStart()
{
    return convertMillisecToString( millisecSinceEpoch() - timestamp_start );
}

unsigned int Clock::millisecSinceEpoch()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string Clock::convertMillisecToString(unsigned int millisec)
{
    double secs = millisec/1000;
    int hours = secs/3600;
    secs -= 3600*hours;
    int minutes = secs/60;
    secs -= 60*minutes;

    return std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string((int)secs);
}
