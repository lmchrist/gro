#ifndef GRAPHICOBSERVER_H
#define GRAPHICOBSERVER_H

#include "DataTabular.h"
#include "DataTextual.h"
#include "ScreenModule.h"
#include "ScreenModuleHandler.h"
#include "Clock.h"
#include <ncurses.h>
#include <thread>
#include <chrono>
#include <unistd.h> //to get process id

// makros for easier data observing
#define GRO_redirectCout();
#define GRO_redirectCerr();
#define GRO_startObserving(); GraphicObserver::getInstance().startObserving();
#define GRO_startGraphics(); GraphicObserver::getInstance().startGraphics();

// makros with arguments
#define GRO_update GraphicObserver::getInstance().getDataTable()->update
#define GRO_ratio GraphicObserver::getInstance().setHorizontalRatio


#include <iostream>
#include <fstream>
#include <stdio.h>



class GraphicObserver
{

public:
    static GraphicObserver& getInstance();
    ~GraphicObserver();

    void startObserving();
    void startGraphics();

    void setHorizontalRatio( double val );

    DataTabular* getDataTable();

    void close();

protected:

private:
    // use GRO as singleton
    GraphicObserver();

    DataTabular* dataTabular = nullptr;
    DataTextual* dataTextual = nullptr;

    ScreenModule* roofModule = nullptr;
    ScreenModule* dataModule = nullptr;
    ScreenModule* textModule = nullptr;
    ScreenModule* fbarModule = nullptr;
    ScreenModuleHandler* screenModuleHandler = nullptr;

    Clock* clock = nullptr;

    void updateGraphics();

    void testOutput();

    unsigned int windowHeight;
    unsigned int windowWidth;

    /// TODO implement different frequencies
    unsigned int graphicUpdatesPerSecond = 2;

    bool graphicsRunning;

    std::thread* graphicUpdater = nullptr;
};

#endif // GRAPHICOBSERVER_H
