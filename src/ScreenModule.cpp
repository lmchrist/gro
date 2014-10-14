#include "ScreenModule.h"
#include <iostream>

ScreenModule::ScreenModule( DataBlock* dataBlock )
{
    if( dataBlock )
    {
        this->dataBlock = dataBlock;
        titlePad = newpad(1, width);
        dataPad  = newpad(height, width);
    }
    else
    {
        /// TODO Error warning
        this->dataBlock = new DataBlock();
    }
    showTitle = true;
    showBody = true;
    secondElapsed = true;
}

ScreenModule::~ScreenModule()
{
    //dtor
}

/// drawers and printers

void ScreenModule::printTitlePad()
{
    if( showTitle )
    {

        if( moduleSelected )
        {
            wbkgd(titlePad, COLOR_PAIR(2));
        }
        else
        {
            wbkgd(titlePad, COLOR_PAIR(1));
        }
        mvwaddstr( titlePad, 0, 1, title.c_str() );
        prefresh( titlePad, 0, 0, posY, posX, posY+1, endX );
    }
}

void ScreenModule::moveCursor( int y, int x )
{
    lines = this->dataBlock->getTotalBreaks();
    if ( lines > 0 )
    {
        //erase old cursor
        eraseCurrentCursor();
        cursorX += x;
        cursorY += y;
        if( cursorX < 0 )
        {
            //cursorX = width - (endX - posX);
            cursorX = 0; // no need for flipping, for now
        }
        else if( cursorX > width - (endX - posX) )
        {
            //cursorX = 0;
            cursorX = width - (endX - posX) ;

        }
        if( cursorY < 0 )
        {
            //cursorY += lines;
            cursorY = 0;
        }
        else if( cursorY >= lines )
        {
            //cursorY -= lines;
            cursorY = lines -1 ;
        }
    }
    drawCurrentCursor();
    printDataPad();
}

void ScreenModule::eraseCurrentCursor()
{
    mvwprintw(dataPad, cursorY, cursorIndent, "   ");
}

void ScreenModule::drawCurrentCursor()
{
    //print new cursor
    wattron(dataPad, A_BOLD | COLOR_PAIR(4));
    mvwprintw(dataPad, cursorY, cursorIndent, ">>>");
    wattroff(dataPad, A_BOLD | COLOR_PAIR(4));
}

void ScreenModule::drawCurrentData()
{
    lines = this->dataBlock->getTotalBreaks();
    if(lines > 0)
    {
        wclear(dataPad);
        drawCurrentCursor();

        std::string tmp;
        unsigned int logicY, logicX;
        unsigned int length;
        unsigned int currentLine = lines - 1; // reverse line order, because unordered_map pushes newest items to front
        DataMap::iterator it;
        // draw every dataItem
        for(it = this->dataBlock->getDataMap()->begin(); it != this->dataBlock->getDataMap()->end(); ++it )
        {
            /// TODO: add drawer for > 1
            for( int i = 0; i < it->second.breaks; i++ )
            {
                // draw current line number
                tmp = std::to_string( currentLine ) + "\t" + it->first + " ";
                length = tmp.length();
                ///TODO cap string at indent length
                for(int j = tabularIndent - length; j > 0; j--)
                {
                    tmp += ".";
                }
                ///TODO method for calc seconds
                if( secondElapsed )
                {
                    // print new update number
                    tmp += " " + std::to_string( it->second.updates );
                    it->second.lastPrintedUpdates = it->second.updates;
                    it->second.updates = 0;
                }
                else
                {
                    // or print last number
                    tmp += " " + std::to_string( it->second.lastPrintedUpdates );
                }

                mvwaddstr( dataPad, currentLine, lineNumberIndent, tmp.c_str() );
                tmp = " | " + it->second.value + "\n";
                waddstr( dataPad, tmp.c_str() );

                currentLine--;
            }
        }

        secondElapsed = false;

    }
}

void ScreenModule::drawCurrentText()
{
    lines = this->dataBlock->getTotalBreaks();
    if(lines > 0)
    {
        wclear(dataPad);
        drawCurrentCursor();
    }
}

void ScreenModule::printDataPad()
{
    if (lines == 0)
    {
        /// do nothing
        //prefresh(module, 0, 0, posY+1, posX, endY, endX);
    }
    else
    {
        //determine dataPad positioning
        int y = cursorY;
        int x = cursorX;
        if (cursorY < endY - posY )
        {
            y = 0;
        }
        else if ( cursorY > lines - (endY - posY) )
        {
            y = lines - (endY - posY);
        }

        if ( cursorX > width - (endX - posX ) - 1 )
        {
            x = width - (endX - posX) - 1;
        }
        // realign dataPad
        prefresh(dataPad, y, x, posY+1, posX, endY, endX);
    }

}

/// setters & getters

void ScreenModule::setPosX( unsigned int val )
{
    posX = val;
}
void ScreenModule::setPosY( unsigned int val )
{
    posY = val;
}
void ScreenModule::setEndX( unsigned int val )
{
    endX = val;
}
void ScreenModule::setEndY( unsigned int val )
{
    endY = val;
}
void ScreenModule::setTitle( std::string title )
{
    this->title = title;
}
void ScreenModule::setID( unsigned int val )
{
    ID = val;
}
void ScreenModule::setTitleVisibility( bool val )
{
    showTitle = val;
}
void ScreenModule::setBodyVisibility( bool val )
{
    showBody = val;
}
void ScreenModule::setSecondElapsed( bool val)
{
    secondElapsed = val;
}

