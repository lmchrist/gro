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

/// makros for easier data observing
#define GRO_redirectCout();
#define GRO_redirectCerr();
#define GRO_startObserving(); GraphicObserver::getInstance().startObserving();  // call this as soon as possible to catch all stream output
#define GRO_startGraphics(); GraphicObserver::getInstance().startGraphics();    // call this to enter graphic mode

// makros with arguments
#define GRO_update GraphicObserver::getInstance().getDataTable()->update        // call GRO_update(name, value); to add data entry
#define GRO_ratio GraphicObserver::getInstance().setHorizontalRatio             // change horizontal split (use before GRO_startGraphics();)


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

    void initNCurses();
    void updateGraphics();


    void testOutput();

    /// TODO implement different frequencies
    unsigned int graphicUpdatesPerSecond = 2;

    bool graphicsRunning;

    std::thread* graphicUpdater = nullptr;
};

#endif // GRAPHICOBSERVER_H
