#include "GraphicObserver.h"
#include <iostream>
GraphicObserver::GraphicObserver()
{
    /// start clock
    CLK_start();

    /// configure text streams before starting ncurse (otherwise freopen() freezes)

    this->dataTabular = new DataTabular();
    this->dataTextual = new DataTextual();

    initNCurses();

    this->screenModuleHandler = new ScreenModuleHandler( 0, 1, 2, 3 ); // init ScreenHandler here to access horizontal ratio

    graphicsRunning = false;
}

GraphicObserver::~GraphicObserver()
{
    this->close();
}

void GraphicObserver::close()
{
    this->stopGraphics();
    delete dataTabular;
    dataTabular = nullptr;
    delete dataTextual;
    dataTextual = nullptr;
    delete screenModuleHandler;
    screenModuleHandler = nullptr;
    std::cout << "GRO: Deleted" << std::endl;
}

void GraphicObserver::stopGraphics()
{
    if( !isendwin() )
    {
        endwin();	// end curses mode
        delete roofModule;
        roofModule = nullptr;
        delete dataModule;
        dataModule = nullptr;
        delete textModule;
        textModule = nullptr;
        delete fbarModule;
        fbarModule = nullptr;
        screenModuleHandler->removeModule();
        std::cout << "GRO: Left graphic mode" << std::endl;
    }
};


void GraphicObserver::initNCurses()
{
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

    init_pair(1, COLOR_BLACK, COLOR_CYAN);  // deactive titleBar
    init_pair(2, -1, COLOR_BLUE); // active titleBar
    init_pair(3, COLOR_RED, -1); // cursor & highlights
    init_pair(4, -1, COLOR_BLACK); // string background
}

void GraphicObserver::configureScreen()
{
    /// add screen modules

    this->roofModule = new ScreenModule( 0, dataTabular, dataTextual );
    this->dataModule = new ScreenModule( 1, dataTabular, dataTextual );
    this->textModule = new ScreenModule( 2, dataTabular, dataTextual );
    this->fbarModule = new ScreenModule( 3, dataTabular, dataTextual );

    screenModuleHandler->addScreenModule(roofModule);
    screenModuleHandler->addScreenModule(dataModule);
    screenModuleHandler->addScreenModule(textModule);
    screenModuleHandler->addScreenModule(fbarModule);

    /// configure screen modules

    this->roofModule->setBodyVisibility(false);
    std::string title = "GRO - Graphic Observer (ProcessID: " + std::to_string(getpid()) + ")";
    this->roofModule->setTitle(title);
    this->roofModule->setModuleSelected(true);

    title = "    #\t        variable                          updates in last sec\t | value";
    this->dataModule->setTitle(title);
    this->dataModule->setModuleSelected(true);

    title = "    #\t        streaming " + dataTextual->getNameOfCurrentPage() + "       ";
    this->textModule->setTitle(title);
    this->textModule->setTextMode(true);

    this->fbarModule->setTitleVisibility(false);

    /// set up bar commands

    Command* quit = new Command( 27, std::bind(&ScreenModuleHandler::dummy, screenModuleHandler), "[Esc]Quit" );
    Command* refresh =  new Command( KEY_F(16), std::bind(&ScreenModuleHandler::reprintScreen, screenModuleHandler), " [F6]Refresh Graphics");
    Command* flipSource = new Command( KEY_F(5), std::bind(&ScreenModule::flipPage, textModule), " [F5]Flip Streams");
    Command* signal = new Command( KEY_F(5), std::bind(&GraphicObserver::generateSignal, this), " [F5]Signal");
    Command* test = new Command( 't', std::bind(&GraphicObserver::testOutput, this), " [t]Debug Output");

    this->fbarModule->addCommand( quit );
    this->fbarModule->addCommand( test );
    this->fbarModule->addCommand( flipSource );
    this->fbarModule->addCommand( refresh );
    //this->fbarModule->addCommand( signal );
}

GraphicObserver& GraphicObserver::getInstance()
{
    // put singleton on heap
    static GraphicObserver* instance = new GraphicObserver();
    return *instance;
}

void GraphicObserver::startGraphics()
{
    testOutput();

    // if graphics aren't already started (to avoid two blocking getch() calls)
    if( !graphicsRunning )
    {
        std::cout << "GRO: Enter graphic mode"<< std::endl;

        graphicsRunning = true;

        configureScreen();

        //add some timing info
        GRO_update("GRO() starttime", CLK_printStartTime());

        // draw everthing
        screenModuleHandler->reprintScreen();

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

            }
            else
            {
                GRO_update("pressed keycode", key);
                screenModuleHandler->callCommands( key );
            }
        }
        ///IMPORTANT: close here, otherwise callCommands will be distorted

        this->stopGraphics();
    }
}

DataTabular* GraphicObserver::getDataTable()
{
    return this->dataTabular;
}

DataTextual* GraphicObserver::getDataTextual()
{
    return this->dataTextual;
}

void GraphicObserver::updateGraphics()
{
    unsigned int wait = 1000 / graphicUpdatesPerSecond;
    bool flipper = true; ///TODO make this more elegant

    int i = 0;
    while( graphicsRunning )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( wait ) );
        GRO_update("time since starttime", CLK_printTimeSinceStart());
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

void GraphicObserver::generateSignal()
{
    std::cout << "GRO: Raised signal"<< std::endl;
    raise (SIGSEGV);
}

void GraphicObserver::testOutput(void)
{
    std::cout << "GRO: std::cout test line"<< std::endl;
    std::cerr << "GRO: std::cerr test line"<< std::endl;
    fprintf(stderr, "GRO: stderr test line\n");
}
