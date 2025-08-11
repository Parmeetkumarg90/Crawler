#ifndef SEO_H
#define SEO_H
#include "../../Day 2-6/library/Crawler.h"
#include "../../Day 2-6/library/String.h"

class SEO
{
private:
    Crawler *obj;
    Character *charObj;

public:
    SEO();
    ~SEO();
    void startCrawling(char *url, char *path,
                       int depth, int maxUrlPerPage);    // used for crawling
    char *getAllData(const char *url, const char *path); // used for reading log file
    char *searchKeyword(char *allData, char *keyword);   // used for getting keyword urls
    void deleteFiles(const char *path);                  // used for removing old data
};

#include "./Seo library.cpp"

#endif