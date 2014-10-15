#include "GraphicObserver.h"

#include <ncurses.h>
#include <stdio.h>

#include <iostream>
#include <fstream>

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

    GRO_update("fire", false);

    GRO_update("what12", -0.343);
           fprintf(stderr, "GRO: derp\n");

   GRO_startGraphics();

/*
    int    fd;
    fpos_t pos;

    printf("stdout, ");

    fflush(stdout);
    fgetpos(stdout, &pos);
    fd = dup(fileno(stdout));
freopen("GRO_stdout.txt", "w", stdout);

       printf("test1\n");
   printf("test2\n");


    fflush(stdout);
    dup2(fd, fileno(stdout));
    close(fd);
    clearerr(stdout);

    printf("stdout again\n");*/


    std::cout << "GRO: Reached end of main() " << std::endl;


    return 0;

}
