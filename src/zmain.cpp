#include "GraphicObserver.h"

#include <ncurses.h>

int main (void) {

    GRO_update("fire", false);
    GRO_update("hoy", 1212);
    GRO_update("what", -0.343);
    GRO_update("what1", -0.343);
    GRO_update("wha2", -0.343);
    GRO_update("wha3t", -0.343);
    GRO_update("what4", -0.343);
    GRO_update("what5", -0.343);
    GRO_update("what6", -0.343);
    GRO_update("what7", -0.343);
    GRO_update("what8", -0.343);
    GRO_update("what9", -0.343);
    GRO_update("what0", -0.343);
    GRO_update("what10", -0.343);
    GRO_update("what11", -0.343);
    GRO_update("what12", -0.343);

    GRO_startGraphics();
    std::cout << "reached return 0 \n" << std::endl;


    return 0;


}
