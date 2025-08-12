#include <iostream>
#include "./library/seo.h"
#include "../Day 2-6/library/String.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (!argv[1] || !argv[2] || !argv[3] || !argv[4])
    {
        cout << "\nFields are missing";
        cout << "Example: ./objectFile https://www.google.com ./pages/temp max-depth(number) pagesPerPage(number)\n\n";
        return 0;
    }
    SEO *obj = new SEO();
    Character *charObj = new Character();
    char keyword[20] = {'\0'}, *allData = nullptr, *keywordUrls = nullptr;
    bool wantCrawl = false, isUrlCrawled = false;
    cout << "\n\t\t\t ==>> Search Engine Optimization <<==";
    cout << "\nEnter keyword to search = ";
    cin >> keyword;
    allData = obj->getAllData(argv[1], argv[2]);
    char *originalAllData = allData;
    if (allData) // url is present
    {
        isUrlCrawled = obj->searchInFile(allData, argv[1]);
    }
    if (isUrlCrawled)
    {
        cout << "\nThis url is already crawler";
        cout << "\nDo you want to crawl again or get already crawled data(1->Crawl,0->Not Crawl) = ";
        cin >> wantCrawl;
        if (wantCrawl)
        {
            obj->startCrawling(argv[1], argv[2], charObj->stringIntoLong(argv[3]), charObj->stringIntoLong(argv[4]));
        }
        keywordUrls = obj->searchKeyword(originalAllData, keyword);
        if (!keywordUrls)
        {
            cout << "\nKeyword is not present";
        }
        else
        {
            cout << "\nAll urls associated with this keyword = ";
            cout << keywordUrls;
        }
    }
    else // url is not present
    {
        obj->startCrawling(argv[1], argv[2], charObj->stringIntoLong(argv[3]), charObj->stringIntoLong(argv[4]));
        allData = obj->getAllData(argv[1], argv[2]);
        originalAllData = allData;
        keywordUrls = obj->searchKeyword(allData, keyword);
        if (!keywordUrls)
        {
            cout << "\nKeyword is not present";
        }
        else
        {
            cout << "\nAll urls associated with this keyword = ";
            cout << keywordUrls;
        }
    }
    delete[] originalAllData;
    delete[] keywordUrls;
    delete obj;
    delete charObj;
    cout << "\n";
    return 0;
}