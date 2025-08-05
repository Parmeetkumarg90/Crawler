#ifndef Header_H
#define Header_H

#include "./HashMap(generic).h"

class Crawler
{
private:
    HashMap<char *, int> *allUrls;                                   // all urls stored in it
    bool isDirectoryPresent(const char *dirPath);                    // is directory present
    void makeDIrectory(const char *dirPath);                         // create a directory
    char *generateUniqueName();                                      // generate a unique name
    char *wgetFileDownload(const char *url, const char *path);       // function to fetch a url
    char *readFile(const char *filePath);                            // read a file
    char **readHtmlUrls(const char *allData, const char *url);       // find all Html or /(at end) urls
    void fileGetDfs(char *url, const char *path, int maxDepthCount); // recursively download html file -> read it -> extract all html anchor tag links -> again repeat until maxDepthCount comes

public:
    Crawler();
    ~Crawler();
    long long stringIntoLong(char *str);                // convert string into number
    void longIntoString(long long num, char *str);      // convert long into string
    void dfs(char *url, char *path, int maxDepthCount); // function for user to use it
};

void my_strcat(char *dest, const char *src);               // concat
int size_tmy_strlen(const char *s);                        // find length
char *my_strstr(const char *haystack, const char *needle); // substring search
void lowercase(const char *ch);                            // convert in lowercase
void my_strcpy(char *dest, const char *src);               // copy src into dest
char *normalizeTextByRemovingSpaces(char *text);           // remove white spaces
char charLowerCase(char c);                                // convert character into lowercase
void clearArrayOfString(char **data);                      // clear a array of character string
void clearCharacters(char *data);                          // clear a array of characters

#include "./Crawler library.cpp"

#endif