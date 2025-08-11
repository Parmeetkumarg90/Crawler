#include <iostream>
#include "./library/seo.h"

using namespace std;

int main()
{
    SEO *obj = new SEO();
    char url[100];
    cout << "\n\t\t\t ==>> Search Engine Optimization <<==";
    cout << "\nEnter url = ";
    cin >> url;
    if (obj->findIfPresent(url)) // url is present
    {
    }
    else // url is not present
    {
    }
    delete obj;
    return 0;
}