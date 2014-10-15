#ifndef SCREENMODULE_H
#define SCREENMODULE_H

#include "DataTabular.h"
#include "DataTextual.h"
#include "Command.h"
#include <list>
#include <iostream>
#include <functional>
#include <ncurses.h>

class ScreenModule
{

public:
    ScreenModule( unsigned int ID, DataTabular* dataTabular = nullptr, DataTextual* dataTextual = nullptr );
    virtual ~ScreenModule();

    // drawers & printers
    void printTitlePad();
    void eraseCurrentCursor();
    void drawCurrentCursor();
    void drawCurrentData();
    void drawCurrentText();
    void printDataPad();
    void printCommands();
    void printModule();

    void setPosX( unsigned int val );
    void setPosY( unsigned int val );
    void setEndX( unsigned int val );
    void setEndY( unsigned int val );
    void setTitle( std::string title );
    void setID( unsigned int val );

    void setTitleVisibility( bool val );
    void setBodyVisibility( bool val );
    void setSecondElapsed( bool val);
    void setModuleSelected( bool val );
    bool isModuleSelected();
    void setTextMode( bool val );
    void tglJumpOnData( );

    void rescale( unsigned int posY, unsigned int posX, unsigned int endY, unsigned int endX );
    void flipPage();

    unsigned int getID();

    void callCommands( int input );
    void addCommand( Command* command );

    unsigned int posX, posY, endX, endY; // module corners (simpler access for ScreenModuleHandler)

private:

    void moveCursor( int y, int x );


    unsigned int lines, ID = 0;
    std::string title;
    bool showTitle, showBody, moduleSelected, textMode, jumpOnData;

    unsigned int width = 1000;   //pad width
    unsigned int height = 10000; //pad height

    int cursorX, cursorY = 0;

    unsigned int cursorIndent = 0;
    unsigned int lineNumberIndent = 5;
    unsigned int tabularIndent = 56;

    bool secondElapsed;

    std::list<Command*> commandList;

    WINDOW* titlePad;
    WINDOW* dataPad;

    DataTabular* dataTabular = nullptr;
    DataTextual* dataTextual = nullptr;
};

#endif // SCREENMODULE_H
