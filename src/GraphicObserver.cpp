#include "GraphicObserver.h"
#include <iostream>
GraphicObserver::GraphicObserver()
{
    /// start clock
    CLK_start();

    /// configure text streams before starting ncurse (otherwise freopen() freezes)

    this->dataTabular = new DataTabular();
    this->dataTextual = new DataTextual();

    this->dataTextual->addTextSource( CERR );
    this->dataTextual->addTextSource( COUT );
    this->dataTextual->changePageTo( COUT );

    /// start ncurses

    initscr(); // start curses mode
    cbreak(); // disable line buffering, do send signals with CTRL+C
    noecho(); // don't show input at cursor
    keypad(stdscr, TRUE);   // to use F keys
    timeout(-1); // getch waits for user input
    if (getenv ("ESCDELAY") == NULL) ESCDELAY = 25; // fixes some internal delay for esc key

    start_color();		    // color mode
    use_default_colors();	// use terminal default colors
    curs_set(0);            // hide blinking cursor

    refresh();	// init call

    /// ncurses color palette

    init_pair(1, COLOR_BLACK, COLOR_CYAN);  // active titleBar
    init_pair(2, -1, COLOR_BLUE); // deactive titleBar
    init_pair(3, COLOR_RED, -1); // cursor
    init_pair(4, COLOR_BLUE, -1); // text highlight

    /// add screen modules

    getmaxyx( stdscr, windowHeight, windowWidth ); // y, x
    graphicsRunning = false;

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

    /// configure screen modules

    this->roofModule->setBodyVisibility(false);
    std::string title = "GRO - Graphic Observer (ProcessID: " + std::to_string(getpid()) + ")";
    this->roofModule->setTitle(title);

    title = "    #\t        variable                             updates last sec\t | value";
    this->dataModule->setTitle(title);
    this->dataModule->setModuleSelected(true);

    title = "   #   Streaming: cout";
    this->textModule->setTitle(title);
    this->textModule->setTextMode(true);

    this->fbarModule->setTitleVisibility(false);


    /// set up bar commands

    Command* test = new Command( 't', std::bind(&GraphicObserver::testOutput, this), " [t]Test");
    Command* quit = new Command( 27, std::bind(&ScreenModuleHandler::dummy, screenModuleHandler), "[Esc]Quit" );

    // set up Bar commands -------------------
    //Command* helpMode = new Command( KEY_F(1), std::bind(&ScreenModuleHandler::switchModules, screenModuleHandler), " [F1]Notes" );
    //Command* controlMode = new Command( KEY_F(2), std::bind(&ScreenModuleHandler::switchModules, screenModuleHandler), " [F2]Controls" );
    //Command* configMode = new Command( KEY_F(3), std::bind(&ScreenModuleHandler::switchModules, screenModuleHandler), " [F3]Config" );
    //Command* controlF4 = new Command( KEY_F(4), std::bind(&DataPort::inputMode, dataPort, screenModuleHandler), " [F4]Parameters", true );
    Command* refresh = new Command( KEY_F(10), std::bind(&ScreenModuleHandler::reprintScreen, screenModuleHandler), " [F10]Refresh Graphics");
    Command* tgglSource = new Command( KEY_F(4), std::bind(&ScreenModule::flipPage, textModule), " [F4]Tggl Text");

    this->fbarModule->addCommand( quit );
    this->fbarModule->addCommand( test );
    this->fbarModule->addCommand( tgglSource );
    this->fbarModule->addCommand( refresh );


    std::cout << "GRO: Initialized"<< std::endl;
}

GraphicObserver::~GraphicObserver()
{
    this->close();
}

void GraphicObserver::close()
{
    std::cout << "GRO: Delete objects" << std::endl;
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
        graphicsRunning = true;

        //add some timing info
        GRO_update("starttime", CLK_printStartTime());

        // draw everthing
        screenModuleHandler->reprintScreen();

        std::cout << "GRO: Start graphic updating"<< std::endl;


        // if updater isn't already running
        if(!graphicUpdater)
        {
            graphicUpdater = new std::thread(std::bind(&GraphicObserver::updateGraphics, this));
        }

        int key;

        std::cout << "GRO: Waiting for keyboard input"<< std::endl;

        while ( graphicsRunning && ( key = getch() ) )
        {
            if(key == 27)
            {
                graphicsRunning = false;
                graphicUpdater->join(); // wait for graphicUpdater to finish
                endwin();	// end curses mode
            }
            else
            {
                GRO_update("pressed keycode", key);
                screenModuleHandler->callCommands( key );
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
        GRO_update("time since program start", CLK_printTimeSinceStart());
        GRO_update("millisec since epoch", CLK_millisecSinceEpoch() );

        flipper = !flipper;
        if(flipper)
        {
            this->dataModule->setSecondElapsed(true);
        }
        this->dataModule->drawCurrentData();
        this->dataModule->printDataPad();

        this->textModule->drawCurrentText();
        this->textModule->printDataPad();
    }
}

void GraphicObserver::setHorizontalRatio(double val)
{
    if(screenModuleHandler)
    {
        screenModuleHandler->setHorizontalRatio(val);
    }
}

void GraphicObserver::testOutput(void)
{
    std::cout << "GRO: test line"<< std::endl;
    std::cerr << "GRO: test line"<< std::endl;
}
