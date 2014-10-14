#ifndef SCREENMODULE_H
#define SCREENMODULE_H

#include "DataBlock.h"
#include <ncurses.h>

class ScreenModule
{

public:
    ScreenModule( DataBlock* dataBlock );
    virtual ~ScreenModule();

    void moveCursor( int y, int x );

    // drawers & printers
    void printTitlePad();
    void eraseCurrentCursor();
    void drawCurrentCursor();
    void drawCurrentData();
    void drawCurrentText();
    void printDataPad();

    void setPosX( unsigned int val );
    void setPosY( unsigned int val );
    void setEndX( unsigned int val );
    void setEndY( unsigned int val );
    void setTitle( std::string title );
    void setID( unsigned int val );

    void setTitleVisibility( bool val );
    void setBodyVisibility( bool val );
    void setSecondElapsed( bool val);

protected:
private:

    unsigned int lines;
    std::string title;
    bool showTitle, showBody, moduleSelected;

    unsigned int width = 1000;   //pad width
    unsigned int height = 10000; //pad height

    unsigned int posX, posY, endX, endY; // module corners

    int cursorX, cursorY = 0;

    unsigned int cursorIndent = 0;
    unsigned int lineNumberIndent = 4;
    unsigned int tabularIndent = 31;

    bool secondElapsed;

    WINDOW* titlePad;
    WINDOW* dataPad;

    DataBlock* dataBlock = nullptr;
};

#endif // SCREENMODULE_H
