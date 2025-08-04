#include <iostream>
#include <cstdlib>
#include "./library/wget.h"
using namespace std;

// example ./run url directory max_depth_crawling
int main(int argc, char *argv[])
{
    char *command = new char[100](), *unqiueName = generateUniqueName(), *space = new char[2]{' ', '\0'};
    if (!argv[1])
    {
        cout << "Please enter a url";
        return 0;
    }
    char urlPrefix[9] = "https://";
    urlPrefix[9] = '\0';
    char *isFound = my_strstr(argv[1], urlPrefix);
    if (!isFound)
    {
        cout << "Url is invalid";
        return 0;
    }
    if (!argv[2])
    {
        cout << "Please enter a path";
        return 0;
    }
    bool isDir = isDirectoryPresent(argv[2]);
    if (!isDir)
    {
        makeDIrectory(argv[2]);
    }
    my_strcat(command, "wget -O");
    my_strcat(command, space);
    my_strcat(command, argv[2]);
    my_strcat(command, unqiueName);
    my_strcat(command, space);
    my_strcat(command, argv[1]);
    cout << command;
    int result = system(command);
    if (result == 0)
    {
        cout << "\nFile Downloaded Success";
        cout << "Path: " << argv[2] << "/" << unqiueName;
    }
    else
    {
        cout << "\nFile Download Failed";
    }
    delete[] command;
    command = nullptr;
    return 0;
}