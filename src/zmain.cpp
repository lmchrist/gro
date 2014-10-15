#include "GraphicObserver.h"

#include <ncurses.h>
#include <stdio.h>
#include <iostream>
#include <fstream>


#include <typeinfo>

void signalHandler( int sig )
{

    GRO_stopGraphics();
    system("clear");
    fprintf( stderr, "SignalHandler reached exit\n" );


    GRO_close();
    printf( "Return to terminal" );
    fflush(stdout);

    exit( 1 );
}

int main(int argc, char* argv[])
{
    // add flag to start GRO optionally
    /* if( argc > 1 )
     {
         for(int i = 0; i < argc; i++)
         {
             if(*argv[i] == '-gro')
             {
                 GRO_startGraphics();
             }
         }
     }*/
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

    GRO_startGraphics();

    // this will be still catch by GRO
    std::cout << "GRO: Reached end of main() " << std::endl;

    GRO_close();
    // this will be printed on your terminal
    std::cout << "Return to terminal" << std::endl;

    return 0;
}
