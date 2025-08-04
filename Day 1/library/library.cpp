#include <iostream>
#include <fstream>
#include <sys/stat.h>
using namespace std;

// Append src to dest
void my_strcat(char *dest, const char *src)
{
    int i, sizeOfSrc = size_tmy_strlen(src), sizeOfDest = size_tmy_strlen(dest);
    for (i = 0; i < sizeOfDest; i++)
    {
    }
    for (int j = 0; j < sizeOfSrc; j++)
    {
        dest[i] = src[j];
        i++;
    }
    dest[i] = '\0';
}

int size_tmy_strlen(const char *s) // return length before '\0'
{
    int count = 0;
    while (s[count] != '\0')
    {
        count++;
    }
    return count;
}

// is directory present
bool isDirectoryPresent(const char *dirPath)
{
    struct stat info;
    return stat(dirPath, &info) == 0 && (info.st_mode & S_IFDIR);
}

// create a directory
void makeDIrectory(const char *dirPath)
{
    if (mkdir(dirPath, 0777) == 0)
    {
        cout << "\nDirectory Created";
    }
    else
    {
        cout << "Failed to create directory";
    }
}