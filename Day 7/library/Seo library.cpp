#include "../../Day 2-6/library/Crawler.h"
#include "../../Day 2-6/library/String.h"
#include "./seo.h"
using namespace std;

SEO::SEO()
{
    obj = new Crawler();
    charObj = new Character();
}

SEO::~SEO()
{
    delete obj;
    delete charObj;
}

// started crawling the website
void SEO::startCrawling(char *url, char *path, int depth, int maxUrlPerPage)
{
    deleteFiles(path);
    obj->dfs(url, path, depth, maxUrlPerPage);
}

char *SEO::getAllData(const char *url, const char *path)
{
    char *newPath = new char[charObj->size_tmy_strlen(path) + 30]();
    charObj->my_strcpy(newPath, path);
    charObj->my_strcat(newPath, "/logFile.txt");
    char *allData = charObj->readFile(newPath);
    charObj->clearCharacters(newPath);
    return allData;
}

char *SEO::searchKeyword(char *allData, char *keyword)
{
    char *searchResult = charObj->my_strstr(allData, keyword);
    if (!searchResult)
    {
        return nullptr;
    }
    int size = charObj->size_tmy_strlen(searchResult) + 1;
    if (allData[charObj->size_tmy_strlen(allData) - size] != '#')
    {
        return nullptr;
    }
    char *processedSearch = new char[size]();
    for (int i = 0; searchResult[i] && searchResult[i] != '#'; i++)
    {
        processedSearch[i] = searchResult[i];
    }
    return processedSearch;
}

bool SEO::searchInFile(char *allData, char *keyword)
{
    return charObj->my_strstr(allData, keyword);
}

void SEO::deleteFiles(const char *path)
{
    try
    {
        if (filesystem::exists(path))
        {
            filesystem::remove_all(path);
        }
    }
    catch (...)
    {
        cout << "Error in file deletion";
    }
}