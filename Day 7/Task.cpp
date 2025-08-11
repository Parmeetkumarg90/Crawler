#include <iostream>
#include "./library/seo.h"

using namespace std;

int main()
{
    SEO *obj = new SEO();
    char url[100] = {'\0'}, path[20] = {'\0'}, keyword[20] = {'\0'}, *allData = nullptr;
    int depth = 0, maxLinkPerPage = 0;
    bool wantCrawl = false;
    cout << "\n\t\t\t ==>> Search Engine Optimization <<==";
    cout << "\nEnter url = ";
    cin >> url;
    cout << "\nEnter path = ";
    cin >> path;
    cout << "\nEnter keyword to search = ";
    cin >> keyword;
    allData = obj->getAllData(url, path);
    if (allData) // url is present
    {
        cout << "\nThis url is already crawler";
        cout << "\nDo you want to crawl again or get already crawled data(1->True,0->False) = ";
        cin >> wantCrawl;
        if (wantCrawl)
        {
            cout << "\nEnter depth = ";
            cin >> depth;
            cout << "\nEnter maxLinkPerPage = ";
            cin >> maxLinkPerPage;
            obj->startCrawling(url, path, depth, maxLinkPerPage);
        }
        char *keywordUrls = obj->getUrls(allData, keyword);
        if (!keywordUrls)
        {
            cout << "\nKeyword is not present";
        }
        else
        {
            cout << "\nAll urls associated with this keyword = ";
            cout << keywordUrls;
        }
        delete[] keywordUrls;
    }
    else // url is not present
    {
        cout << "\nEnter depth = ";
        cin >> depth;
        cout << "\nEnter maxLinkPerPage = ";
        cin >> maxLinkPerPage;
        obj->startCrawling(url, path, depth, maxLinkPerPage);
        allData = obj->getAllData(url, path);
        char *keywordUrls = obj->getUrls(allData, keyword);
        if (!keywordUrls)
        {
            cout << "\nKeyword is not present";
        }
        else
        {
            cout << "\nAll urls associated with this keyword = ";
            cout << keywordUrls;
        }
        delete[] keywordUrls;
    }
    delete[] allData;
    delete obj;
    cout << "\n\n";
    return 0;
}