#include "Clock.h"

bool Clock::instanceExists = false;
Clock* Clock::clock = NULL;

Clock::Clock()
{
    startPoint = std::chrono::system_clock::now();
}

Clock::~Clock()
{
    instanceExists = false;
}

Clock* Clock::getInstance()
{
    if(!instanceExists)
    {
        clock = new Clock();
        instanceExists = true;
        return clock;
    }
    else
    {
        return clock;
    }
}
