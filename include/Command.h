#ifndef COMMAND_H
#define COMMAND_H
#include <functional>

class Command
{
public:
    Command( int _key, std::function<void( )> _routine, std::string _description = "" ) : key(_key), routine(_routine), description(_description) {}

    int returnKey( void )
    {
        return key;
    }
    std::string returnDescription( void )
    {
        return description;
    }
    void execute( void )
    {
        routine( );
    }

private:
    int key;
    std::function<void( )> routine;
    std::string description;
};

#endif // COMMAND_H
