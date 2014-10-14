#include "DataTextual.h"

std::thread* DataTextual::streamObserver;

DataTextual::DataTextual()
{

    if(!streamObserver)
    {
        DataTextual::streamObserver = new std::thread(std::bind(&DataTextual::updatePages, this));
    }
    addTextSource(EMPTY);
}

DataTextual::~DataTextual()
{
    keepRunning = false;
    DataTextual::streamObserver->join();
    if(coutbuf)
    {
        f_cout.close();
        std::cout.rdbuf(coutbuf);
    }
}

Book* DataTextual::getBook()
{
    return &book;
}

void DataTextual::addTextSource( TextSource src )
{
    Book::iterator it = book.find(src);
    if( it == book.end() )
    {
        Page page;
        std::pair<TextSource, Page> element (src, page);
        book.insert( element );

        /// setup streams
        if(src == COUT)
        {
            f_cout.open("GRO_cout.txt"); // member to keep buffer from being destroyed out of scope
            coutbuf = std::cout.rdbuf(); //save old buf
            std::cout.rdbuf(f_cout.rdbuf()); //redirect std::cout to cout.txt
        }
    }
}

void DataTextual::changePageTo( TextSource src )
{
    Book::iterator it = book.find(src);
    if( it != book.end() )
    {
        currentSource = src;
    }
}

void DataTextual::updatePages()
{
    while( this->keepRunning )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

        /// TODO check, that current page isnt end
        if(currentSource == COUT)
        {
            /// TODO exctract open/close
            std::ifstream myfile;
            std::string line;
            myfile.open ("GRO_cout.txt");

            if (myfile.is_open())
            {
                while ( getline (myfile,line) )
                {
                    book.at(currentSource).push_back(line);
                }
            }
            myfile.close();
            std::ofstream ofs;
            ofs.open("GRO_cout.txt", std::ofstream::out | std::ofstream::trunc);    //erase file content
            ofs.close();
        }
    }
}

Page* DataTextual::getCurrentPage()
{
    Book::iterator it = book.find( currentSource );
    if(it != book.end())
    {
        return &(book.at(currentSource));
    }
    return nullptr;
}

