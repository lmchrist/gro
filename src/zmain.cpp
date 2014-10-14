#include "GraphicObserver.h"

#include <ncurses.h>

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
   GRO_startGraphics();
    std::cout << "reached return 0 \n" << std::endl;


    return 0;

}
