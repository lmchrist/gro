#include "ScreenModule.h"
#include "GraphicObserver.h"
ScreenModule::ScreenModule( unsigned int ID, DataTabular* dataTabular, DataTextual* dataTextual )
{
    if( dataTabular )
    {
        this->dataTabular = dataTabular;
    }
    if( dataTextual )
    {
        this->dataTextual = dataTextual;
    }

    this->ID = ID;
    this->titlePad = newpad(1, width);
    this->dataPad  = newpad(height, width);

    this->moduleSelected = false;
    this->showTitle = true;
    this->showBody = true;
    this->secondElapsed = true;
    this->textMode = false;
    this->jumpOnData = false;

    /// add commands

    Command* cursorUp = new Command(KEY_UP, std::bind(&ScreenModule::moveCursor, this, -1, 0), "");
    Command* cursorDown = new Command(KEY_DOWN, std::bind(&ScreenModule::moveCursor, this,1, 0), "" );
    Command* cursorLeft = new Command(KEY_LEFT, std::bind(&ScreenModule::moveCursor, this,0, -3), "" );
    Command* cursorRight = new Command(KEY_RIGHT, std::bind(&ScreenModule::moveCursor, this, 0, 3), "" );
    Command* cursorTop = new Command(KEY_HOME, std::bind(&ScreenModule::moveCursor, this, -height, 0), "" );
    Command* cursorBottom = new Command(KEY_END, std::bind(&ScreenModule::moveCursor, this, height, 0), "" );

    this->addCommand( cursorUp );
    this->addCommand( cursorDown );
    this->addCommand( cursorLeft );
    this->addCommand( cursorRight );
    this->addCommand( cursorTop );
    this->addCommand( cursorBottom );
}

ScreenModule::~ScreenModule()
{
    //dtor
}
unsigned int ScreenModule::getID()
{
    return this->ID;
}

void ScreenModule::callCommands( int input )
{
    for( auto it = commandList.begin(); it != commandList.end(); it++ )
    {
        if( input == (*it)->returnKey() )
        {
            (*it)->execute();
        }
    }
}

void ScreenModule::addCommand( Command* command )
{
    commandList.push_back( command );
}

/// drawers and printers

void ScreenModule::printTitlePad()
{
    if( this->showTitle )
    {

        if( moduleSelected )
        {
            wbkgd(titlePad, COLOR_PAIR(2));
        }
        else
        {
            wbkgd(titlePad, COLOR_PAIR(1));
        }

        //determine titlePad positioning
        int x = cursorX;
        if ( cursorX > width - (endX - posX ) - 1 )
        {
            x = width - (endX - posX) - 1;
        }
        // realign titlePad
        mvwaddstr( titlePad, 0, 1, title.c_str() );
        prefresh( titlePad, 0, x, posY, posX, posY+1, endX );
    }
}

void ScreenModule::moveCursor( int y, int x )
{
    if(this->moduleSelected)
    {
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
        printTitlePad();
        printDataPad();
    }
}

void ScreenModule::eraseCurrentCursor()
{
    mvwprintw(dataPad, cursorY, cursorIndent, "    ");
}

void ScreenModule::drawCurrentCursor()
{
    //print new cursor
    wattron(dataPad, A_BOLD | COLOR_PAIR(3));
    mvwprintw(dataPad, cursorY, cursorIndent, " >>>");
    wattroff(dataPad, A_BOLD | COLOR_PAIR(3));
}

void ScreenModule::drawCurrentData()
{
    if( this->showBody &&  this->dataTabular && !this->textMode )
    {
        lines = this->dataTabular->getTotalBreaks();
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
            for(it = this->dataTabular->getDataMap()->begin(); it != this->dataTabular->getDataMap()->end(); ++it )
            {
                /// TODO: add drawer for > 1
                for( int i = 0; i < it->second.breaks; i++ )
                {
                    // draw current line number
                    tmp = std::to_string( currentLine ) + "   \t" + it->first + " ";
                    length = tmp.length();
                    ///TODO cap string at indent length
                    for(int j = tabularIndent - length + std::to_string( currentLine ).length() -1; j > 0; j--) //currentLine fixes decimal positions
                    {
                        tmp += ".";
                    }
                    mvwaddstr( this->dataPad, currentLine, lineNumberIndent, tmp.c_str() );

                    ///TODO method for calc seconds
                    if( secondElapsed )
                    {
                        // print new update number
                        tmp = " " + std::to_string( it->second.updates );
                        it->second.lastPrintedUpdates = it->second.updates;
                        it->second.updates = 0;
                    }
                    else
                    {
                        // or print last number
                        tmp = " " + std::to_string( it->second.lastPrintedUpdates );
                    }
                    // updates > 0 in cyan
                    if( it->second.lastPrintedUpdates > 0)
                    {
                        wattron(dataPad, A_BOLD | COLOR_PAIR(3));
                        waddstr(dataPad, tmp.c_str());
                        wattroff(dataPad, A_BOLD | COLOR_PAIR(3));
                    }
                    else
                    {
                        // else normal color
                        waddstr(dataPad, tmp.c_str());
                    }
                    tmp = " \t | ";
                    waddstr( dataPad, tmp.c_str() );

                    tmp = it->second.value;

                    // value with brackground, to see whitspaces
                    wattron( dataPad, COLOR_PAIR( 4 ) );
                    waddstr( dataPad, tmp.c_str() );
                    wattroff( dataPad, COLOR_PAIR( 4 ) );

                    tmp = "\n";
                    waddstr( dataPad, tmp.c_str() );

                    currentLine--;
                }
            }

            secondElapsed = false;

        }
    }
}

void ScreenModule::drawCurrentText()
{
    if(this->showBody &&  this->dataTextual && this->textMode)
    {
        Page* page = this->dataTextual->getCurrentPage();
        if(page)
        {
            std::string tmp, val;
            unsigned int currentLine = 0;

            if( page->size() > 0 )
            {
                wclear(dataPad);
                if( jumpOnData )
                {
                    moveCursor(height,cursorX);
                }
                else
                {
                    moveCursor(0,cursorX);
                }
                for(auto it = page->begin(); it != page->end(); it++ )
                {
                    tmp = std::to_string( currentLine ) + "   \t" + *it + "\n";
                    mvwaddstr( this->dataPad, currentLine, lineNumberIndent, tmp.c_str() );
                    currentLine++;
                }
            }
            this->lines = currentLine;
        }
    }
}

void ScreenModule::printDataPad()
{
    if(this->showBody)
    {
        if (this->lines == 0)
        {
            /// TODO add lear line
            wclear(dataPad);
            prefresh(dataPad, 0, 0, posY+1, posX, endY, endX);
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
}

void ScreenModule::printCommands()
{
    if(this->showBody)
    {
        size_t pos;
        wmove( dataPad, 0, 0 ); //mv logic cursor to 0,0
        std::string description, tmp;

        for( auto it = commandList.begin(); it != commandList.end(); it++ )
        {
            description = ( *it )->returnDescription();

            //TODO handle errors
            pos = description.find( "]" );
            tmp = description.substr( 0, pos + 1 );
            description.erase( 0, pos + 1 );

            waddstr( dataPad, tmp.c_str() );

            wattron( dataPad, COLOR_PAIR( 1 ) );
            waddstr( dataPad, description.c_str() );
            wattroff( dataPad, COLOR_PAIR( 1 ) );
        }
        unsigned int y, x;
        getyx( dataPad, y, x ); // get logic cursor position
        mvwchgat( dataPad, 0, x, -1, COLOR_PAIR( 1 ), 1, NULL ); // print end bar

        this->lines = 1;
    }
    printDataPad();
}

void ScreenModule::printModule()
{
    if( this->textMode )
    {
        drawCurrentText();
    }
    else
    {
        drawCurrentData();
    }
    printDataPad();
    printTitlePad();
}

void ScreenModule::flipPage()
{
    if( this->textMode && this->dataTextual )
    {
        dataTextual->flipPage();

        ///TODO change title
        title = "    #\t        streaming " + dataTextual->getNameOfCurrentPage() + "       ";

        //draw text first to get new line counting
        drawCurrentText();
        // update cursor
        moveCursor(height - 1,0); ///NOTE: maybe move cursor down
        printDataPad();
        printTitlePad();
    }
}

/// setters & getters

void ScreenModule::rescale( unsigned int _posY, unsigned int _posX, unsigned int _endY, unsigned int _endX )
{
    posX = _posX;
    posY = _posY;
    endX = _endX;
    endY = _endY;
}

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
void ScreenModule::setModuleSelected( bool val)
{
    moduleSelected = val;
}
void ScreenModule::setTextMode( bool val )
{
    textMode = val;
}
bool ScreenModule::isModuleSelected()
{
    return moduleSelected;
}
void ScreenModule::tglJumpOnData()
{
   jumpOnData = !jumpOnData;
    GRO_update("jumpOnData", jumpOnData); //NOTE: for debug
}
