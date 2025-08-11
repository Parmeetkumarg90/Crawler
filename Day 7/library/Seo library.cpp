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
    return charObj->readFile(path);
}

char *SEO::getUrls(char *allData, char *keyword)
{
    charObj->lowercase(keyword);
    charObj->lowercase(allData);
    for (int i = 0; allData[i]; i++)
    {
        if (allData[i] == '#')
        {
            bool isFound = charObj->startsWith(&allData[i + 1], keyword);
            if (isFound)
            {
                while (allData[i] != '\0' && allData[i] != '>')
                {
                    i++;
                }
                int start = i + 1, end = -1;
                while (allData[i] != '\0' && allData[i] != '#')
                {
                    i++;
                }
                end = i - 1;
                if (end > start)
                {
                    char *allUrls = new char[end - start + 2]();
                    int j = 0;
                    while (start <= end)
                    {
                        allUrls[j++] = allData[start++];
                    }
                    allUrls[j] = '\0';
                    return allUrls;
                }
            }
        }
    }
    return nullptr;
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