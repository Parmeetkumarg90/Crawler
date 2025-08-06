#ifndef Header_H
#define Header_H

#include "./HashMap(generic).h"
#include "./String.h"

class Crawler
{
private:
    Character *charObj;                                                                   // used for all string operations
    HashMap<char *, int> *allUrls;                                                        // all urls stored in it
    bool isDirectoryPresent(const char *dirPath);                                         // is directory present
    void makeDIrectory(const char *dirPath);                                              // create a directory
    char *generateUniqueName();                                                           // generate a unique name
    char *wgetFileDownload(const char *url, const char *path);                            // function to fetch a url
    char *readFile(const char *filePath);                                                 // read a file
    char **readHtmlUrls(const char *allData, const char *url);                            // find all Html or /(at end) urls
    void fileGetDfs(char *url, const char *path, int maxDepthCount, int maxFoundPerPage); // recursively download html file -> read it -> extract all html anchor tag links -> again repeat until maxDepthCount comes

public:
    Crawler();
    ~Crawler();
    void dfs(char *url, char *path, int maxDepthCount, int maxFoundPerPage); // function for user to use it
};

#include "./Crawler library.cpp"

#endif