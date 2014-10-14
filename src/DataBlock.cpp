#include "DataBlock.h"
#include <iostream>
DataBlock::DataBlock()
{
}

DataBlock::~DataBlock()
{
    //dtor
}

DataMap* DataBlock::getDataMap()
{
    return &dataMap;
}

unsigned int DataBlock::getTotalBreaks()
{
    return totalBreaks;
}


void DataBlock::update( const std::string& key, const std::string& value, const unsigned int& breaks )
{
    DataMap::iterator it = dataMap.find( key );
    if ( it != dataMap.end() )
    {
        it->second.value = value;
        it->second.updates++;
    }
    else
    {
        // use copy insertion
        DataItem item(value, breaks);
        std::pair<const std::string&, DataItem&> element ( key, item );
        dataMap.insert( element );
        totalBreaks += breaks;
    }
}



/// methods to update single values

void DataBlock::update( const std::string& key, const bool& value )
{
    update( key, translate(value) );
}
void DataBlock::update( const std::string& key, const int& value )
{
    update( key, translate(value) );
}
void DataBlock::update( const std::string& key, const long int& value )
{
    update( key, translate(value) );
}
void DataBlock::update( const std::string& key, const unsigned int& value )
{
    update( key, translate(value) );
}
void DataBlock::update( const std::string& key, const unsigned long int& value )
{
    update( key, translate(value) );
}
void DataBlock::update( const std::string& key, const float& value )
{
    update( key, translate(value) );
}
void DataBlock::update( const std::string& key, const double& value )
{
    update( key, translate(value) );
}
void DataBlock::update( const std::string& key, const long double& value )
{
    update( key, translate(value) );
}



/// methods to translate single values

std::string DataBlock::translate( const bool& value )
{
    if( value )
        return "true";
    else
        return "false";
}
std::string DataBlock::translate( const int& value )
{
    return std::to_string( value );
}
std::string DataBlock::translate( const long int& value )
{
    return std::to_string( value );
}
std::string DataBlock::translate( const unsigned int& value )
{
    return std::to_string( value );
}
std::string DataBlock::translate( const unsigned long int& value )
{
    return std::to_string( value );
}
std::string DataBlock::translate( const float& value )
{
    return std::to_string( value );
}
std::string DataBlock::translate( const double& value )
{
    return std::to_string( value );
}
std::string DataBlock::translate( const long double & value )
{
    return std::to_string( value );
}
