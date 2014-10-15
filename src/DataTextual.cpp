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
        fflush(stdout);
        dup2(fd_stdout, fileno(stdout));
        close(fd_stdout);
        clearerr(stdout);
        fsetpos(stdout, &pos_stdout);
    }
    if(book.find(STDERR)!=book.end())
    {
        fflush(stderr);
        dup2(fd_stderr, fileno(stderr));
        close(fd_stderr);
        clearerr(stderr);
        fsetpos(stderr, &pos_stderr);
    }
    /// NOTE: maybe copy files before deleting
    /*
    remove("GRO_cout.txt");
    remove("GRO_cerr.txt");
    remove("GRO_stdout.txt");
    remove("GRO_stderr.txt");
    */
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
        /*
        if(src == STDOUT)
        {
            fflush(stdout);
            fgetpos(stdout, &pos_stdout);
            fd_stdout = dup(fileno(stdout));
            freopen("GRO_stdout.txt", "w", stdout);
        }*/
        if(src == STDERR)
        {
            fflush(stderr);
            fgetpos(stderr, &pos_stderr);
            fd_stderr = dup(fileno(stderr));
            freopen("GRO_stderr.txt", "w", stderr);
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
        std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

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


        if(currentSource == DMESG)
        {
            system("dmesg -T > GRO_dmesg.txt");
            /// TODO exctract open/close

            std::ifstream myfile;
            std::string line;
            myfile.open ("GRO_dmesg.txt");

            if (myfile.is_open())
            {
                unsigned int i = book.at(DMESG).size();
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

        if(currentSource == STDERR)
        {
    fprintf(stderr, "GRO: This is a stderr check\n");
            fflush(stderr);

            /// TODO exctract open/close
            std::ifstream myfile;
            std::string line;
            myfile.open ("GRO_stderr.txt");

            if (myfile.is_open())
            {

                unsigned int i = book.at(STDERR).size();

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
    case STDOUT:
        return "stdout";
    case STDERR:
        return "stderr";
    case DMESG:
        return "dmesg";
    }
}

