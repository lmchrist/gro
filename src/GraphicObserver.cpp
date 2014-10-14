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

    /// add screen modules

    getmaxyx( stdscr, windowHeight, windowWidth ); // y, x
    graphicsRunning = false;

    this->dataTabular = new DataTabular();
    this->dataTextual = new DataTextual();

    this->roofModule = new ScreenModule( 0, dataTabular, dataTextual );
    this->dataModule = new ScreenModule( 1, dataTabular, dataTextual );
    this->textModule = new ScreenModule( 2, dataTabular, dataTextual );
    this->fbarModule = new ScreenModule( 3, dataTabular, dataTextual );

    this->screenModuleHandler = new ScreenModuleHandler( 0, 1, 2, 3 );

    screenModuleHandler->addScreenModule(roofModule);
    screenModuleHandler->addScreenModule(dataModule);
    screenModuleHandler->addScreenModule(textModule);
    screenModuleHandler->addScreenModule(fbarModule);

    screenModuleHandler->setHorizontalRatio(0.5);

    /// configure text streams

    this->dataTextual->addTextSource( COUT );

    /// configure screen modules

    this->roofModule->setBodyVisibility(false);
    std::string title = "GRO - Graphic Observer (ProcessID: " + std::to_string(getpid()) + ")";
    this->roofModule->setTitle(title);

    title = "   #   variable       updates last sec | value";
    this->dataModule->setTitle(title);

    title = "   #   Streaming: cout";
    this->textModule->setTitle(title);
    this->textModule->setTextMode(true);

    this->fbarModule->setTitleVisibility(false);


    /// set up bar commands

    // set up Bar commands -------------------
    //Command* helpMode = new Command( KEY_F(1), std::bind(&ScreenModuleHandler::switchModules, screenModuleHandler), " [F1]Notes" );
    //Command* controlMode = new Command( KEY_F(2), std::bind(&ScreenModuleHandler::switchModules, screenModuleHandler), " [F2]Controls" );
    //Command* configMode = new Command( KEY_F(3), std::bind(&ScreenModuleHandler::switchModules, screenModuleHandler), " [F3]Config" );
    //Command* controlF4 = new Command( KEY_F(4), std::bind(&DataPort::inputMode, dataPort, screenModuleHandler), " [F4]Parameters", true );
    //Command* switchMode = new Command( KEY_F(9), std::bind(&RobotControlsKeyboard::switchControlMode, robotControlsKeyboard, S91), " [F9]Switch Mode", true );

    Command* quit = new Command( 27, std::bind(&ScreenModuleHandler::dummy, screenModuleHandler), " [Esc]Quit" );

    this->fbarModule->addCommand( quit );

}

GraphicObserver::~GraphicObserver()
{
    this->close();
}

void GraphicObserver::close()
{
    std::cout << "delete objects \n" << std::endl;
    delete dataTabular;
    delete dataTextual;
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
        // draw everthing
        screenModuleHandler->reprintScreen();

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
            default:
            {
                screenModuleHandler->callCommands( key );
            }
            }
        }
        ///IMPORTANT: close here, otherwise callCommands will be distorted

        this->close();
    }
}

DataTabular* GraphicObserver::getDataTable()
{
    return this->dataTabular;
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

        this->textModule->drawCurrentText();
        this->textModule->printDataPad();
    std::cout << "drawn\n" << std::endl;

    }
}

void GraphicObserver::setHorizontalRatio(double val)
{
    if(screenModuleHandler)
    {
        screenModuleHandler->setHorizontalRatio(val);
    }
}
