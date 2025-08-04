#include <iostream>
#include <cstdlib>
#include "./library/wget.h"
using namespace std;

// example ./run url directory max_depth_crawling
int main(int argc, char *argv[])
{
    if (!argv[1] || !argv[2] || !argv[3])
    {
        cout << "\nFields are missing";
        cout << "Example: ./objectFile https://www.google.com ./pages/temp 5";
        return 0;
    }
    fileGetDfs(argv[1], argv[2], stringIntoLong(argv[3]));
    return 0;
}