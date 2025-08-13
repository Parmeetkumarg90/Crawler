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
    if (!allData || !keyword)
    {
        return nullptr;
    }
    char *searchResult = charObj->my_strstr(allData, keyword);
    if (!searchResult)
    {
        return nullptr;
    }
    int size = charObj->size_tmy_strlen(searchResult);
    if (size <= 0)
    {
        return nullptr;
    }
    int allDataSize = charObj->size_tmy_strlen(allData);
    if (allDataSize < size || allData[allDataSize - size] != '#')
    {
        return nullptr;
    }
    char *processedSearch = new char[size + 1]();
    int i = 0;
    for (i = 0; searchResult[i] && searchResult[i] != '#'; i++)
    {
        processedSearch[i] = searchResult[i];
    }
    processedSearch[i] = '\0';
    return processedSearch;
}

bool SEO::searchInFile(char *allData, char *keyword)
{
    return (charObj->my_strstr(allData, keyword) != nullptr);
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