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
    obj->dfs(url, path, depth, maxUrlPerPage);
}

char *SEO::getAllData(const char *url, const char *path)
{
    return charObj->readFile(path);
}

char *SEO::getUrls(const char *allData, const char *keyword)
{
    for (int i = 0; allData[i]; i++)
    {
        if (allData[i] == '#')
        {
            bool isFound = charObj->startsWith(&allData[i + 1], keyword);
            if (isFound)
            {
                while (allData[i] != '>')
                {
                    i++;
                }
                int start = i, end = -1;
                while (allData[i] != '#')
                {
                    i++;
                }
                end = i - 1;
                if (end > start)
                {
                    char *allUrls = new char[end - start + 2]();
                    int i = 0;
                    while (start <= end)
                    {
                        allUrls[i++] = allData[start++];
                    }
                    allUrls[i] = '\0';
                    return allUrls;
                }
            }
        }
    }
    return nullptr;
}

void deleteFiles(const char *path)
{
}