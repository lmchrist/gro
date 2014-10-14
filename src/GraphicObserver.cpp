#include "GraphicObserver.h"
#include <iostream>
GraphicObserver::GraphicObserver()
{
    /// start ncurses

    initscr(); // start curses mode
    cbreak(); // disable line buffering, do send signals with CTRL+C
    noecho(); // don't show input at cursor
    keypad(stdscr, TRUE);   // to use F keys
    timeout(-1); // getch waits for user input
    if (getenv ("ESCDELAY") == NULL) ESCDELAY = 25; // fixes some internal delay for esc key

    start_color();		    // color mode
    use_default_colors();	// use terminal default colors
    //curs_set(0); // hide blinking cursor

    refresh();	// init call

    /// ncurses color palette

    init_pair(1, COLOR_BLACK, COLOR_CYAN);  // active titleBar
    init_pair(2, COLOR_BLACK, COLOR_GREEN); // deactive titleBar

    /// init screen modules

    getmaxyx( stdscr, windowHeight, windowWidth ); // y, x
    graphicsRunning = false;

    this->dataBlock = new DataBlock();
    this->roofModule = new ScreenModule( dataBlock );
    this->dataModule = new ScreenModule( dataBlock );
    this->textModule = new ScreenModule( dataBlock );
    this->fbarModule = new ScreenModule( dataBlock );

    this->roofModule->setBodyVisibility(false);
    this->roofModule->setPosX(0);
    this->roofModule->setPosY(0);
    this->roofModule->setEndX(windowWidth -1);
    this->roofModule->setEndY(windowHeight -1);
    std::string title = "GRO - Graphic Observer (ProcessID: " + std::to_string(getpid()) + ")";
    this->roofModule->setTitle(title);

    this->dataModule->setPosX(0);
    this->dataModule->setPosY(2);
    this->dataModule->setEndX(windowWidth  -1);
    this->dataModule->setEndY(windowHeight -10);
    title = "   #   variable       updates last sec | value";
    this->dataModule->setTitle(title);

    this->textModule->setPosX(0);
    this->textModule->setPosY(windowHeight - 8);
    this->textModule->setEndX(windowWidth - 1);
    this->textModule->setEndY(windowHeight - 1);
    title = "   #   Streaming: cout";
    this->textModule->setTitle(title);
}

GraphicObserver::~GraphicObserver()
{
    std::cout << "called destructor \n" << std::endl;
    delete dataBlock;
    delete roofModule;
    delete dataModule;
    delete textModule;
    delete fbarModule;
}

GraphicObserver& GraphicObserver::getInstance()
{
    // put singleton on heap
    static GraphicObserver* instance = new GraphicObserver();
    return *instance;
}

void GraphicObserver::startObserving() {}

void GraphicObserver::startGraphics()
{
    // if graphics aren't already started (to avoid two blocking getch() calls)
    if( !graphicsRunning )
    {
        ///TODO drawAll()
        this->roofModule->printTitlePad();

        this->dataModule->printTitlePad();
        this->dataModule->drawCurrentData();
        this->dataModule->printDataPad();

        this->textModule->printTitlePad();
        this->textModule->drawCurrentData();
        this->textModule->printDataPad();

        graphicsRunning = true;

        // if updater isn't already running
        if(!graphicUpdater)
        {
            graphicUpdater = new std::thread(std::bind(&GraphicObserver::updateGraphics, this));
        }

        char key;

        while ( graphicsRunning && ( key = getch() ) )
        {
            switch (key)
            {
            case 27: /* user pressed backspace */
            {
                graphicsRunning = false;
                graphicUpdater->join(); // wait for graphicUpdater to finish
                endwin();	// end curses mode
            }
            }
        }
        std::cout << "end of getch \n" << std::endl;
    }
}

DataBlock* GraphicObserver::getDataTable()
{
    return this->dataBlock;
}

void GraphicObserver::updateGraphics()
{
    unsigned int wait = 1000 / graphicUpdatesPerSecond;
    bool flipper = true; ///TODO make this more elegant

    int i = 0;
    while( graphicsRunning )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( wait ) );
        GRO_update("half secs", i++);

        flipper = !flipper;
        if(flipper)
        {
            this->dataModule->setSecondElapsed(true);
        }

        this->dataModule->drawCurrentData();
        this->dataModule->printDataPad();

        this->textModule->drawCurrentData();
        this->textModule->printDataPad();
    }
}

