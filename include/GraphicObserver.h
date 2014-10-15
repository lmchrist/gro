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
#include <signal.h> //to generate signals

/// makros to make it simple

#define GRO();               GraphicObserver::getInstance();                    // init GRO
#define GRO_startGraphics(); GraphicObserver::getInstance().startGraphics();    // call this to enter graphic mode
#define GRO_stopGraphics();  GraphicObserver::getInstance().stopGraphics();     // call this to leave graphic mode
#define GRO_close();         GraphicObserver::getInstance().close();            // destroy GRO

// set text sources
#define GRO_read_cout();    GraphicObserver::getInstance().getDataTextual()->addTextSource( COUT );
#define GRO_read_cerr();    GraphicObserver::getInstance().getDataTextual()->addTextSource( CERR );
#define GRO_read_stderr();  GraphicObserver::getInstance().getDataTextual()->addTextSource( STDERR );
#define GRO_read_dmesg();   GraphicObserver::getInstance().getDataTextual()->addTextSource( DMESG );

#define GRO_read_allSources();  GRO_read_cerr(); GRO_read_stderr(); GRO_read_dmesg(); GRO_read_cout();

// makros with arguments
#define GRO_update(x,y);    GraphicObserver::getInstance().getDataTable()->update(x,y);        // call GRO_update(name, value) to update data entry
#define GRO_ratio(x);       GraphicObserver::getInstance().setHorizontalRatio(x);              // change horizontal split
#define GRO_savepath(x);    GraphicObserver::getInstance().getDataTextual()->setBackupPath(x);   // path to save streams


#include <iostream>
#include <fstream>
#include <stdio.h>



class GraphicObserver
{

public:
    static GraphicObserver& getInstance();
    ~GraphicObserver();

    void startGraphics();
    void stopGraphics();

    void setHorizontalRatio( double val );

    DataTabular* getDataTable();
    DataTextual* getDataTextual();

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
    void configureScreen();

    void updateGraphics();


    void testOutput();
void generateSignal();

    /// TODO implement different frequencies
    unsigned int graphicUpdatesPerSecond = 2;

    bool graphicsRunning;

    std::thread* graphicUpdater = nullptr;
};

#endif // GRAPHICOBSERVER_H
