#ifndef DATATEXTUAL_H
#define DATATEXTUAL_H

#include<list>
#include<map>
#include<thread>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <ctime>

#include <string>

enum TextSource { EMPTY, COUT, CERR, DMESG, STDERR, STDOUT, LOGGER };

typedef std::list<std::string> Page;
typedef std::map<TextSource, Page> Book;

class DataTextual
{
public:
    DataTextual();
    virtual ~DataTextual();

    void addTextSource( TextSource src );
    Page* getCurrentPage();
    std::string getNameOfCurrentPage();
    void changePageTo( TextSource src );
    std::string generateFilenameOf( TextSource src );

    void flipPage();

    void setBackupPath(std::string);
    void saveSource( TextSource src );

    Book* getBook();

protected:
private:
    void updatePages();

    void observeSource(TextSource src, unsigned int offset);

    Book book;
    TextSource currentSource;

    bool keepRunning = true;
    static std::thread* streamObserver;

    std::string backupPath = ".";

    // COUT
    std::ofstream f_cout;
    std::streambuf *coutbuf = nullptr;

    // COUT
    std::ofstream f_cerr;
    std::streambuf *cerrbuf = nullptr;

    //STDOUT
    fpos_t pos_stdout;
    int fd_stdout;

        //STDOUT
    fpos_t pos_stderr;
    int fd_stderr;

};

#endif // DATATEXTUAL_H
