#ifndef SCREENMODULEHANDLER_H
#define SCREENMODULEHANDLER_H

#include "Command.h"
#include "ScreenModule.h"
#include <list>
#include <map>

class ScreenModuleHandler
{
public:

    ScreenModuleHandler( unsigned int roofID, unsigned int dataID, unsigned int textID, unsigned int fbarID ); // ids roof, data, text, fbar ScreenModule

    void addScreenModule( ScreenModule* screenModule );
    void addCommand( Command* command );

    void reprintScreen();

    void callCommands( char input );

    void dummy(){}

    void setHorizontalRatio( double );

private:

    void jumpModules();
    void swapModules();
    void changeHorizontalRatio( int );

    unsigned int roofID;
    unsigned int upperID;
    unsigned int lowerID;
    unsigned int fbarID;
    unsigned int activeScreenModuleID;

    std::list<unsigned int> screenModuleIDs;
    std::list<Command*> commandList;
    std::map<unsigned int, ScreenModule*> screenModules;

    unsigned int terminalX, terminalY;

    double horizontalRatio = 0.6;

};

#endif // SCREENMODULEHANDLER_H
