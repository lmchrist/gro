#ifndef DATATEXTUAL_H
#define DATATEXTUAL_H

#include<list>
#include<map>
#include<thread>

#include <iostream>
#include <fstream>
#include <stdio.h>

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
    void flipPage();

    Book* getBook();

protected:
private:
    void updatePages();
    Book book;
    TextSource currentSource;

    bool keepRunning = true;
    static std::thread* streamObserver;

    // COUT
    std::ofstream f_cout;
    std::streambuf *coutbuf = nullptr;

    // COUT
    std::ofstream f_cerr;
    std::streambuf *cerrbuf = nullptr;

    //STDOUT
    fpos_t pos_stdout;
    int fd_stdout;

};

#endif // DATATEXTUAL_H
