#include "GraphicObserver.h"

#include <ncurses.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>

void signalHandler( int sig )
{
    GRO_stopGraphics();
    system("clear");

    /**
    *
    *   do stuff logged in GRO files
    *
    **/

    GRO_close();

    /**
    *
    *   do stuff logged in terminal
    *
    **/

    printf( "Return to terminal\n" );
    fflush(stdout);

    exit( 1 );
}

int main(int argc, char* argv[])
{
    /// TODO: finish signals
    signal( SIGSEGV, signalHandler );
    signal( SIGABRT, signalHandler );
    signal( SIGTERM, signalHandler );

    GRO();
    GRO_read_allSources();
    GRO_ratio(0.7);
    GRO_savepath(".");

    GRO_update("a wild bool", true);
    GRO_update("you are here:", "earth");
    GRO_update("avogadro constant", 6.022e+23);
    GRO_update("an int", 1);
    GRO_update("string with exceeding whitespaces", "txt     ");

    GRO_update("argc",argc);

    // start GRO optionally via -gro flag
    for(int i = 0; i < argc; i++)
    {
        if(strcmp( argv[i], "-gro") == 0)
        {
            GRO_startGraphics();
        }
    }

    // this will be still catch by GRO
    std::cout << "GRO: Reached end of main() " << std::endl;

    GRO_close();

    // this will be printed on terminal
    std::cout << "Return to terminal" << std::endl;

    return 0;
}
