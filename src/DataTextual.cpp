#include "DataTextual.h"
#include "GraphicObserver.h"
std::thread* DataTextual::streamObserver;

DataTextual::DataTextual()
{

    if(!streamObserver)
    {
        DataTextual::streamObserver = new std::thread(std::bind(&DataTextual::updatePages, this));
    }
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
    if(cerrbuf)
    {
        f_cerr.close();
        std::cout.rdbuf(cerrbuf);
    }
    if(book.find(STDOUT)!=book.end())
    {
//Close file and restore standard output to stdout
        fclose(stdout);
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
        if(src == CERR)
        {
            f_cerr.open("GRO_cerr.txt"); // member to keep buffer from being destroyed out of scope
            cerrbuf = std::cerr.rdbuf(); //save old buf
            std::cerr.rdbuf(f_cerr.rdbuf()); //redirect std::cout to cout.txt
        }
        if(src == STDOUT)
        {
            //Redirect current standard output
            freopen("GRO_stdout.txt", "rw", stdout);
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

void DataTextual::flipPage()
{
    Book::iterator it = book.find(currentSource);

    if( ++it == book.end() )
    {
        currentSource = book.begin()->first;
    }
    else
    {
        currentSource = it->first;
    }
}

void DataTextual::updatePages()
{
    while( this->keepRunning )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

        /// TODO check, that current page isnt end
        if(currentSource == COUT)
        {
            /// TODO exctract open/close
            std::ifstream myfile;
            std::string line;
            myfile.open ("GRO_cout.txt");

            if (myfile.is_open())
            {
                unsigned int i = book.at(COUT).size();
                // jump over old logs
                while(i > 0)
                {
                    getline (myfile,line);
                    i--;
                }
                // save new ones
                while ( getline (myfile,line) )
                {
                    book.at(currentSource).push_back(line);
                }
            }

        }

        if(currentSource == CERR)
        {
            /// TODO exctract open/close
            std::ifstream myfile;
            std::string line;
            myfile.open ("GRO_cerr.txt");

            if (myfile.is_open())
            {
                unsigned int i = book.at(CERR).size();
                // jump over old logs
                while(i > 0)
                {
                    getline (myfile,line);
                    i--;
                }
                // save new ones
                while ( getline (myfile,line) )
                {
                    book.at(currentSource).push_back(line);
                }
            }

        }

        /*
                if(currentSource == STDOUT)
                {
                    /// TODO exctract open/close
                    std::ifstream myfile;
                    std::string line;
                    myfile.open ("GRO_stdout.txt");

                    if (myfile.is_open())
                    {
                        while ( getline (myfile,line) )
                        {
                            book.at(currentSource).push_back(line);
                        }
                    }
                    // clear file
                    remove( "GRO_stdout.txt" );
                }*/
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
std::string DataTextual::getNameOfCurrentPage()
{
    switch(currentSource)
    {
    case COUT:
        return "cout";
    case CERR:
        return "cerr";
    }
}

