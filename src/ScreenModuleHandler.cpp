#include "ScreenModuleHandler.h"

ScreenModuleHandler::ScreenModuleHandler( unsigned int roofID, unsigned int upperID, unsigned int lowerID, unsigned int fbarID )
{
    this->roofID = roofID;
    this->upperID = upperID;
    this->lowerID = lowerID;
    this->fbarID = fbarID;
    this->activeScreenModuleID = upperID;
    screenModuleIDs.push_back(roofID);
    screenModuleIDs.push_back(upperID);
    screenModuleIDs.push_back(lowerID);
    screenModuleIDs.push_back(fbarID);

    /// TODO problem?
   // screenModules.at(this->activeScreenModuleID)->setModuleSelected(true);

    /// add commands

    Command* jumping = new Command( '\t', std::bind(&ScreenModuleHandler::jumpModules, this) );
    Command* swapping = new Command( 'c', std::bind(&ScreenModuleHandler::swapModules, this) );
    Command* horizontalUp = new Command( KEY_PPAGE, std::bind(&ScreenModuleHandler::changeHorizontalRatio, this, -3));
    Command* horizontalDown = new Command( KEY_NPAGE, std::bind(&ScreenModuleHandler::changeHorizontalRatio, this, 3));

    this->addCommand( jumping );
    this->addCommand( swapping );
    this->addCommand( horizontalUp );
    this->addCommand( horizontalDown );
}

void ScreenModuleHandler::setHorizontalRatio( double val )
{
    horizontalRatio = val;
    /// NOTE: maybe redraw screen?
}

void ScreenModuleHandler::addScreenModule( ScreenModule* screenModule )
{
    screenModules.insert( std::pair<int, ScreenModule*>( screenModule->getID(), screenModule) );
}

void ScreenModuleHandler::addCommand( Command* command )
{
    commandList.push_back( command );
}

void ScreenModuleHandler::callCommands( char input )
{
    for( auto it = commandList.begin(); it != commandList.end(); it++ )
    {
        // call routines
        if( input == (*it)->returnKey() )
        {
            (*it)->execute();
        }
    }
    // check through modules
    for( auto it = screenModuleIDs.begin(); it!= screenModuleIDs.end(); it++ )
    {
            screenModules.at(*it)->callCommands(input );
    }
}

void ScreenModuleHandler::reprintScreen(  )
{
    refresh(); // to get updated terminal size

    getmaxyx( stdscr, terminalY, terminalX ); //get terminal size

    ScreenModule* moduleR = screenModules.at( roofID );
    ScreenModule* moduleU = screenModules.at( upperID );
    ScreenModule* moduleL = screenModules.at( lowerID );
    ScreenModule* moduleB = screenModules.at( fbarID );

    unsigned int h = terminalX*horizontalRatio;   // horizontal split position

    /// TODO erase screen

    moduleR->rescale( 0, 0, 1, terminalX-1 );
    moduleU->rescale( 3, 0, h-1, terminalX - 1 );
    moduleL->rescale( h+1, 0, terminalY-2, terminalX-1 );
    moduleB->rescale( terminalY-2, 0, terminalY-1, terminalX-1);

    moduleR->printModule();
    moduleU->printModule();
    moduleL->printModule();
    moduleB->printCommands();
}

void ScreenModuleHandler::swapModules( )
{
    ScreenModule* moduleA = screenModules.at( upperID );
    ScreenModule* moduleB = screenModules.at( lowerID );

    /// TODO: write getters
    unsigned int tmpPosX = moduleA->posX;
    unsigned int tmpPosY = moduleA->posY;
    unsigned int tmpEndX = moduleA->endX;
    unsigned int tmpEndY = moduleA->endY;

    moduleA->rescale( moduleB->posY, moduleB->posX, moduleB->endY, moduleB->endX );
    moduleB->rescale( tmpPosY, tmpPosX, tmpEndY, tmpEndX );

    moduleA->printModule();
    moduleB->printModule();

    unsigned int tmpID = upperID;
    upperID = lowerID;
    lowerID = tmpID;
}


void ScreenModuleHandler::jumpModules(  )
{
    ScreenModule* moduleA = screenModules.at( upperID );
    ScreenModule* moduleB = screenModules.at( lowerID );

    if ( moduleA->isModuleSelected() )
    {
        moduleA->setModuleSelected(false);
        moduleB->setModuleSelected(true);
        moduleA->printTitlePad();
        moduleB->printTitlePad();
        activeScreenModuleID = lowerID;
    }
    else
    {
        moduleA->setModuleSelected(true);
        moduleB->setModuleSelected(false);
        moduleA->printTitlePad();
        moduleB->printTitlePad();
        activeScreenModuleID = upperID;
    }
}


void ScreenModuleHandler::changeHorizontalRatio( int modifier )
{
    ScreenModule* modulA = screenModules.at(upperID);
    ScreenModule* modulB = screenModules.at(lowerID);

    if (modifier > 0)   // make upper modul bigger
    {
        if(modulB->endY - modulB->posY <= modifier )
        {
            modulB->posY = modulB->endY - 1;
            modulA->endY = modulB->posY - 1;
        }
        else
        {
            modulB->posY += modifier;
            modulA->endY += modifier;
        }
    }

    else if (modifier < 0)  // make upper modul smaller
    {
        if(modulA->endY - modulA->posY <= -modifier )
        {
            modulA->endY = modulA->posY + 1;
            modulB->posY = modulA->endY + 1;
        }
        else
        {
            modulB->posY += modifier;
            modulA->endY += modifier;
        }
    }
    modulA->printModule();
    modulB->printModule();


    //refresh();
}

