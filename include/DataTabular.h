#ifndef DataTabular_H
#define DataTabular_H

#include <unordered_map>
#include <iostream>
#include <mutex>

struct DataItem
{
    std::string value;
    unsigned int updates; //reset after printing
    unsigned int breaks;  //size of item (>1 for matrices)
    unsigned int lastPrintedUpdates;

    DataItem( std::string _value )
    {
        DataItem( _value, 1);
    }
    DataItem( std::string _value, unsigned int _breaks )
    {
        value = _value;
        updates = 1;
        breaks = _breaks;
        lastPrintedUpdates = 1;
    }
};

typedef std::unordered_map<std::string, DataItem>  DataMap;

class DataTabular
{
public:
    DataTabular();
    virtual ~DataTabular();

    DataMap* getDataMap();
    unsigned int getTotalBreaks();

    void update( const std::string& key, const std::string& value, const unsigned int& breaks = 1 );

    void update( const std::string& key, const char*  value );
    void update( const std::string& key, const bool& value );
    void update( const std::string& key, const int& value );
    void update( const std::string& key, const long int& value );
    void update( const std::string& key, const unsigned int& value );
    void update( const std::string& key, const unsigned long int& value );
    void update( const std::string& key, const float& value );
    void update( const std::string& key, const double& value );
    void update( const std::string& key, const long double& value );

protected:
private:

    DataMap dataMap;

    unsigned int totalBreaks = 0;

    std::string translate( const bool& value );
    std::string translate( const int& value );
    std::string translate( const long int& value );
    std::string translate( const unsigned int& value );
    std::string translate( const unsigned long int& value );
    std::string translate( const float& value );
    std::string translate( const double& value );
    std::string translate( const long double& value );

    std::mutex updateGuard;
};

#endif // DataTabular_H
