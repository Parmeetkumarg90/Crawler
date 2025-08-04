#include <iostream>
#include <cstdlib>
#include "./library/wget.h"
using namespace std;

// example ./run url directory max_depth_crawling
int main(int argc, char *argv[])
{
    char *command = new char[100](), filename[20] = "/output.html ";
    if (!argv[1])
    {
        cout << "Please enter a url";
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
    my_strcat(command, "wget -O ");
    my_strcat(command, argv[2]);
    my_strcat(command, filename);
    my_strcat(command, argv[1]);
    int result = system(command);
    if (result == 0)
    {
        cout << "\nFile Downloaded SUccess";
    }
    else
    {
        cout << "\nFile Download Failed";
    }
    delete[] command;
    command = nullptr;
    return 0;
}