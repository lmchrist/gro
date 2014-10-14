#ifndef GRAPHICOBSERVER_H
#define GRAPHICOBSERVER_H

#include "DataBlock.h"
#include "ScreenModule.h"
#include <ncurses.h>
#include <thread>
#include <chrono>
#include <unistd.h> //to get process id

// makros for easier data observing
#define GRO_startObserving GraphicObserver::getInstance().startObserving
#define GRO_startGraphics GraphicObserver::getInstance().startGraphics
#define GRO_update GraphicObserver::getInstance().getDataTable()->update

class GraphicObserver
{

public:
    static GraphicObserver& getInstance();
    ~GraphicObserver();

    void startObserving();
    void startGraphics();

    DataBlock* getDataTable();

protected:

private:
    // use GRO as singleton
    GraphicObserver();

    DataBlock* dataBlock = nullptr;
    ScreenModule* roofModule = nullptr;
    ScreenModule* dataModule = nullptr;
    ScreenModule* textModule = nullptr;
    ScreenModule* fbarModule = nullptr;

    void updateGraphics();

    unsigned int windowHeight;
    unsigned int windowWidth;

    /// TODO implement different frequencies
    unsigned int graphicUpdatesPerSecond = 2;

    bool graphicsRunning;

    std::thread* graphicUpdater = nullptr;
};

#endif // GRAPHICOBSERVER_H
