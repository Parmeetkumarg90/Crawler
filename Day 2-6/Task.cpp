#include <iostream>
#include <cstdlib>
#include "./library/Crawler.h"
#include "./library/String.h"
using namespace std;

// example ./run url directory max_depth_crawling
int main(int argc, char *argv[])
{
    if (!argv[1] || !argv[2] || !argv[3] || !argv[4])
    {
        cout << "\nFields are missing";
        cout << "Example: ./objectFile https://www.google.com ./pages/temp max-depth(number) pagesPerPage(number)";
        return 0;
    }
    Crawler *obj = new Crawler();
    Character *charObj = new Character();
    obj->dfs(argv[1], argv[2], charObj->stringIntoLong(argv[3]), charObj->stringIntoLong(argv[4]));
    delete obj;
    delete charObj;
    cout << "\n";
    return 0;
}