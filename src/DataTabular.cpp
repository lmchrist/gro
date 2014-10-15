#include "DataTabular.h"
#include <iostream>
DataTabular::DataTabular()
{
}

DataTabular::~DataTabular()
{
    //dtor
}

DataMap* DataTabular::getDataMap()
{
    return &dataMap;
}

unsigned int DataTabular::getTotalBreaks()
{
    return totalBreaks;
}


void DataTabular::update( const std::string& key, const std::string& value, const unsigned int& breaks )
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
void DataTabular::update( const std::string& key, const char*  value )
{
    update( key, (std::string)value );
}
void DataTabular::update( const std::string& key, const bool& value )
{
    update( key, translate(value) );
}
void DataTabular::update( const std::string& key, const int& value )
{
    update( key, translate(value) );
}
void DataTabular::update( const std::string& key, const long int& value )
{
    update( key, translate(value) );
}
void DataTabular::update( const std::string& key, const unsigned int& value )
{
    update( key, translate(value) );
}
void DataTabular::update( const std::string& key, const unsigned long int& value )
{
    update( key, translate(value) );
}
void DataTabular::update( const std::string& key, const float& value )
{
    update( key, translate(value) );
}
void DataTabular::update( const std::string& key, const double& value )
{
    update( key, translate(value) );
}
void DataTabular::update( const std::string& key, const long double& value )
{
    update( key, translate(value) );
}



/// methods to translate single values

std::string DataTabular::translate( const bool& value )
{
    if( value )
        return "true";
    else
        return "false";
}
std::string DataTabular::translate( const int& value )
{
    return std::to_string( value );
}
std::string DataTabular::translate( const long int& value )
{
    return std::to_string( value );
}
std::string DataTabular::translate( const unsigned int& value )
{
    return std::to_string( value );
}
std::string DataTabular::translate( const unsigned long int& value )
{
    return std::to_string( value );
}
std::string DataTabular::translate( const float& value )
{
    return std::to_string( value );
}
std::string DataTabular::translate( const double& value )
{
    return std::to_string( value );
}
std::string DataTabular::translate( const long double & value )
{
    return std::to_string( value );
}
