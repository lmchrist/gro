#include "GraphicObserver.h"

#include <ncurses.h>

int main (void) {

    GRO_update("fire", false);
    GRO_update("hoy", 1212);
    GRO_update("what", -0.343);

    GRO_startGraphics();
    std::cout << "reached return 0 \n" << std::endl;


    return 0;


}
