#include "Clock.h"

Clock::Clock()
{
    clockStart = std::chrono::system_clock::now();
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
    std::time_t t = std::chrono::system_clock::to_time_t(clockStart);
    char mbstr[100];
    if (std::strftime(mbstr, sizeof(mbstr), "%T", std::localtime(&t))) {
        return mbstr;
    }
    return "";
}

std::string Clock::printTimeSinceStart()
{
    auto millisec = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - clockStart);
    return convertMillisecToString( millisec.count() );

}

unsigned int Clock::millisecSinceEpoch()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string Clock::convertMillisecToString(unsigned int millisec)
{
    ///NOTE: only for values smaller than one day
    ///TODO: make this faster
    double secs = millisec/1000;
    int hours = secs/3600;
    secs -= 3600*hours;
    int minutes = secs/60;
    secs -= 60*minutes;
    std::string tmp;
    if( hours < 10)
        tmp+= "0";
    tmp+=std::to_string(hours)+ ":";
    if( minutes < 10)
        tmp+= "0";
    tmp += std::to_string(minutes) + ":";
    if(secs < 10)
        tmp+="0";
    tmp +=std::to_string((int)secs);
    return tmp;
}
