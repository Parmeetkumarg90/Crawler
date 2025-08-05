#include <iostream>
#include <cstdlib>
#include "./library/Crawler.h"
using namespace std;

// example ./run url directory max_depth_crawling
int main(int argc, char *argv[])
{
    Crawler *obj = new Crawler();
    if (!argv[1] || !argv[2] || !argv[3])
    {
        cout << "\nFields are missing";
        cout << "Example: ./objectFile https://www.google.com ./pages/temp 5";
        return 0;
    }
    obj->dfs(argv[1], argv[2], obj->stringIntoLong(argv[3]));
    delete obj;
    cout << "\n";
    return 0;
}