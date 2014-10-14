#ifndef DATATEXTUAL_H
#define DATATEXTUAL_H

#include<list>
#include<map>
#include<thread>

#include <iostream>
#include <fstream>

#include <string>

enum TextSource { EMPTY, COUT, CERR, DMESG, STDERR, STDCOUT, LOGGER };

typedef std::list<std::string> Page;
typedef std::map<TextSource, Page> Book;

class DataTextual
{
public:
    DataTextual();
    virtual ~DataTextual();

    void addTextSource( TextSource src );
    Page* getCurrentPage();
    void changePageTo( TextSource src );

    Book* getBook();

protected:
private:
    void updatePages();

    std::ofstream f_cout;

    Book book;
    TextSource currentSource;

    bool keepRunning = true;
    static std::thread* streamObserver;

    std::streambuf *coutbuf = nullptr;

};

#endif // DATATEXTUAL_H
