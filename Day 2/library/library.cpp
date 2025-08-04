#include <iostream>
#include <fstream>
#include <ctime>
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
                    delete[] newhaystack;
                    delete[] newneedle;
                    return (char *)&haystack[i];
                }
            }
        }
    }
    delete[] newhaystack;
    delete[] newneedle;
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
    char *uniqueName = new char[30];
    time_t timenow = time(0);
    longIntoString(timenow, uniqueName);
    cout << uniqueName;
    return uniqueName;
}

// convert long into string
void longIntoString(long long num, char *str)
{
    str[0] = '/';
    if (num == 0)
    {
        str[1] = '0';
        str[2] = '\0';
    }
    else
    {
        int i = 1, rem = 0;
        if (num < 0)
        {
            num = -num;
            str[i] = '-';
            i++;
        }
        long long revNum = 0;
        while (num > 0)
        {
            rem = num % 10;
            revNum = revNum * 10 + rem;
            num /= 10;
        }
        while (revNum > 0)
        {
            str[i] = (revNum % 10) + '0';
            revNum /= 10;
            i++;
        }
        str[i] = '\0';
    }
    char extension[] = {'.', 'h', 't', 'm', 'l', '\0'};
    my_strcat(str, extension);
}

// convert string into number
long long stringIntoLong(char *str)
{
    int size = size_tmy_strlen(str), i = 0;
    long long num = 0;
    bool isNegative = false;
    if (str[i] == '-')
    {
        isNegative = true;
        i++;
    }
    for (; i < size; i++)
    {
        num = num * 10 + (str[i] - '0');
    }
    return num;
}

// function to fetch a url
char *wgetFileDownload(const char *url, char *path)
{
    char *command = new char[100](), *unqiueName = generateUniqueName(), *space = new char[2]{' ', '\0'};
    if (!url)
    {
        cout << "Please enter a url";
        return nullptr;
    }
    char urlPrefix[9] = "https://";
    urlPrefix[9] = '\0';
    char *isFound = my_strstr(url, urlPrefix);
    if (!isFound)
    {
        cout << "Url is invalid";
        return nullptr;
    }
    if (!path)
    {
        cout << "Please enter a path";
        return nullptr;
    }
    bool isDir = isDirectoryPresent(path);
    if (!isDir)
    {
        makeDIrectory(path);
    }
    my_strcat(command, "wget -O");
    my_strcat(command, space);
    my_strcat(command, path);
    my_strcat(command, unqiueName);
    my_strcat(command, space);
    my_strcat(command, url);
    cout << command;
    int result = system(command);
    if (result == 0)
    {
        cout << "\nFile Downloaded Success";
        cout << "Path: " << path << "/" << unqiueName;
    }
    else
    {
        cout << "\nFile Download Failed";
    }
    delete[] command;
    command = nullptr;
    delete[] space;
    space = nullptr;
    delete[] unqiueName;
    unqiueName = nullptr;
    char diff[2] = {'/', '\0'};
    my_strcat(path, diff);
    my_strcat(path, unqiueName);
    return path;
}

// read a file
char *readFile(const char *filePath)
{
    fstream file(filePath, ios::in);
    if (!file.is_open())
    {
        return nullptr;
    }
    int i = 0;
    char ch, *allData = new char[1000];
    while (file.get(ch))
    {
        allData[i] = ch;
        i++;
    }
    allData[i] = '\0';
    file.close();

    return allData;
}

// recursively download html file -> read it -> extract all html anchor tag links -> again repeat until maxDepthCount comes
void fileGetDfs(const char *url, char *path, int maxDepthCount)
{
    if (!url || !path || !maxDepthCount || maxDepthCount <= 0)
    {
        return;
    }
    char *currFilePath = wgetFileDownload(url, path);
    const char *allData = readFile(currFilePath);

    delete[] allData;
}