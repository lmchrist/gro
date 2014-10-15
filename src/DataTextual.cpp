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

    // backup and delete stream files
    for(auto it = book.begin(); it!= book.end(); it++)
    {
        this->saveSource( it->first );
        remove(generateFilenameOf( it->first ).c_str());
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
            f_cout.open(generateFilenameOf(COUT)); // member to keep buffer from being destroyed out of scope
            coutbuf = std::cout.rdbuf(); //save old buf
            std::cout.rdbuf(f_cout.rdbuf()); //redirect std::cout to cout.txt
        }
        if(src == CERR)
        {
            f_cerr.open(generateFilenameOf(CERR)); // member to keep buffer from being destroyed out of scope
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
            freopen(generateFilenameOf(STDERR).c_str(), "w", stderr);
        }
        if(src == DMESG)
        {
            std::string command = "tailf /var/log/syslog > " + generateFilenameOf(DMESG) + " &";
            ///TODO: check if tailf installed
            system( command.c_str() );
        }

    }
    changePageTo(src);
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
        switch(currentSource)
        {
        case COUT:
            observeSource(COUT, 0);
            break;
        case CERR:
            observeSource(CERR, 0);
            break;
        case DMESG:
            observeSource(DMESG, 0);
            break;
        case STDERR:
            fflush(stderr);
            observeSource(STDERR, 0);
            break;
        }
    }
}
void DataTextual::observeSource(TextSource src, unsigned int offset)
{

    std::ifstream myfile;
    std::string line;
    myfile.open (generateFilenameOf(src));

    if (myfile.is_open())
    {
        unsigned int i = book.at(src).size() + offset;
        // jump over old logs
        while(i > 0)
        {
            getline (myfile,line);
            i--;
        }
        // save new ones
        while ( getline (myfile,line) )
        {
            book.at(src).push_back(line);
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
    default:
        return "";
    }
}

std::string DataTextual::generateFilenameOf( TextSource src )
{
    switch(src)
    {
    case COUT:
        return "GRO_cout.txt";
    case CERR:
        return "GRO_cerr.txt";
    case STDOUT:
        return "GRO_stdout.txt";
    case STDERR:
        return "GRO_stderr.txt";
    case DMESG:
        return "GRO_dmesg.txt";
    default:
        return "";
    }
}

void DataTextual::saveSource( TextSource src )
{
    std::ifstream  srcFile(generateFilenameOf(src));
    if(srcFile.is_open())
    {
        std::string path = backupPath + "/BACKUP_" + generateFilenameOf(src);
        std::ofstream  dstFile;
        dstFile.open (path, std::fstream::in | std::fstream::out | std::fstream::app);
        time_t now = time(0);
        char* dt = ctime(&now);

        std::string time = (std::string)dt;
        time.erase(time.find_last_not_of(" \n\r\t")+1);

        std::string tmp = "[    New GRO at: " + time +"    ] \n\n";
        dstFile << tmp;
        dstFile << srcFile.rdbuf();
        dstFile << "\n\n\n";
        dstFile.close();
    }
}

void DataTextual::setBackupPath(std::string path)
{
    backupPath = path;
}

