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

// return length before '\0'
int size_tmy_strlen(const char *s)
{
    int count = 0;
    while (s[count] != '\0')
    {
        count++;
    }
    return count;
}

// convert into lower case
void lowercase(char *ch)
{
    if (!ch)
        return;
    int i = 0;
    while (ch[i] != '\0')
    {
        if (ch[i] >= 'A' && ch[i] <= 'Z')
        {
            ch[i] += 32;
        }
        i++;
    }
}

// Copy src into dest
char *my_strcpy(char *dest, const char *src)
{
    int i, sizeOfSrc = size_tmy_strlen(src);
    for (i = 0; i < sizeOfSrc; i++)
    {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return dest;
}

// Substring search
char *my_strstr(const char *haystack, const char *needle)
{
    char *newhaystack = my_strcpy(new char[size_tmy_strlen(haystack) + 1], haystack);
    char *newneedle = my_strcpy(new char[size_tmy_strlen(haystack) + 1], needle);
    lowercase(newhaystack);
    lowercase(newneedle);
    int sizeOfNeedle = size_tmy_strlen(newneedle), sizeOfHayStack = size_tmy_strlen(newhaystack);
    for (int i = 0; i < sizeOfHayStack; i++)
    {
        for (int j = 0; j < sizeOfNeedle; j++)
        {
            if (newhaystack[i] == newneedle[j])
            {
                int compareLength = 0, traverI = i;
                for (int k = 0; k < sizeOfNeedle; k++)
                {
                    if (newhaystack[traverI] == newneedle[k])
                    {
                        traverI++;
                        compareLength++;
                    }
                    else
                    {
                        k = sizeOfNeedle;
                    }
                }
                if (compareLength == sizeOfNeedle)
                {
                    return (char *)&newhaystack[i];
                }
            }
        }
    }
    return nullptr;
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

// generate a unique name
char *generateUniqueName()
{
    char *uniqueName = new char[20]{'/', 'u', 'n', 'i', 'q', 'u', 'e', '.', 'h', 't', 'm', 'l', '\0'};
    return uniqueName;
}