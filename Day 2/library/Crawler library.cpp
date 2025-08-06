#include <iostream>
#include <fstream>
#include <ctime>
#include <filesystem>
#include "./Crawler.h"
using namespace std;

Crawler::Crawler()
{
    allUrls = new HashMap<char *, int>();
    charObj = new Character();
}

Crawler::~Crawler()
{
    delete allUrls;
}

// is directory present
bool Crawler::isDirectoryPresent(const char *dirPath)
{
    return filesystem::exists(dirPath);
}

// create a directory
void Crawler::makeDIrectory(const char *dirPath)
{
    if (filesystem::create_directories(dirPath))
    {
        cout << "\nDirectory Created";
    }
    else
    {
        cout << "Failed to create directory";
    }
}

// generate a unique name
char *Crawler::generateUniqueName()
{
    char *uniqueName = new char[30];
    time_t timenow = time(0);
    charObj->longIntoString(timenow, uniqueName);
    // cout << uniqueName;
    return uniqueName;
}

// function to fetch a url
char *Crawler::wgetFileDownload(const char *url, const char *path)
{
    char *command = new char[100](), *unqiueName = generateUniqueName(), *space = new char[2]{' ', '\0'};
    if (!url)
    {
        cout << "Please enter a url";
        return nullptr;
    }
    char urlPrefix[5] = "http";
    urlPrefix[5] = '\0';
    char *isFound = charObj->my_strstr(url, urlPrefix);
    if (!isFound)
    {
        cout << "\nUrl is invalid: " << url;
        return nullptr;
    }
    if (!path)
    {
        cout << "\nPlease enter a path";
        return nullptr;
    }
    bool isDir = isDirectoryPresent(path);
    if (!isDir)
    {
        makeDIrectory(path);
    }
    charObj->my_strcat(command, "wget -O");
    charObj->my_strcat(command, space);
    charObj->my_strcat(command, path);
    charObj->my_strcat(command, unqiueName);
    charObj->my_strcat(command, space);
    charObj->my_strcat(command, url);
    // cout << command;
    int result = system(command);
    if (result == 0)
    {
        cout << "\nFile Downloaded Success";
        cout << "Path: " << path << "/" << unqiueName;
    }
    else
    {
        cout << "\nFile Download Failed";
        return nullptr;
    }
    char *newPath = new char[charObj->size_tmy_strlen(path) + charObj->size_tmy_strlen(unqiueName) + 2];
    charObj->my_strcpy(newPath, path);
    charObj->my_strcat(newPath, "/");
    charObj->my_strcat(newPath, unqiueName);
    charObj->clearCharacters(unqiueName);
    charObj->clearCharacters(command);
    charObj->clearCharacters(space);
    return newPath;
}

// read a file
char *Crawler::readFile(const char *filePath)
{
    fstream file(filePath, ios::in);
    if (!file.is_open())
    {
        return nullptr;
    }
    int i = 0;
    char ch, *allData = new char[100000];
    while (file.get(ch))
    {
        if (i >= 99990)
        {
            break;
        }
        allData[i] = ch;
        i++;
    }
    allData[i] = '\0';
    file.close();

    return allData;
}

// function for user to use it
void Crawler::dfs(char *url, char *path, int maxDepthCount, int maxFoundPerPage)
{
    if (!url || !path || !maxDepthCount || maxDepthCount <= 0)
    {
        return;
    }
    allUrls->hashInsertion(url, 0);
    fileGetDfs(url, path, maxDepthCount, maxFoundPerPage);
}

// recursively download html file -> read it -> extract all html anchor tag links -> again repeat until maxDepthCount comes
void Crawler::fileGetDfs(char *url, const char *path, int maxDepthCount, int maxFoundPerPage)
{
    if (!url || !path || !maxDepthCount || maxDepthCount <= 0)
    {
        return;
    }
    Node<char *, int> *currNodeUrl = allUrls->getNode(url);
    if (currNodeUrl && currNodeUrl->getValue() == 1)
    {
        return;
    }
    else if (currNodeUrl)
    {
        currNodeUrl->setVal(1);
    }
    char *currFilePath = wgetFileDownload(url, path);
    if (!currFilePath)
    {
        return;
    }
    // cout << currFilePath;
    char *allData = readFile(currFilePath);
    if (!allData)
    {
        charObj->clearCharacters(currFilePath);
        return;
    }
    // cout << allData;
    allData = charObj->normalizeTextByRemovingSpaces(allData);
    char **thisPageUrl = readHtmlUrls(allData, url, maxFoundPerPage);
    for (int i = 0; thisPageUrl[i] && i < maxDepthCount; i++)
    {
        allUrls->hashInsertion(thisPageUrl[i], 0);
        fileGetDfs(thisPageUrl[i], path, maxDepthCount - 1, maxDepthCount);
    }
    charObj->clearArrayOfString(thisPageUrl);
    charObj->clearCharacters(allData);
    charObj->clearCharacters(currFilePath);
}

// find all urls
char **Crawler::readHtmlUrls(const char *allData, const char *url, int maxFoundPerPage)
{
    char **thisPageUrls = new char *[maxFoundPerPage]();
    int startIndex = 0, urlIndex = 0;
    // bool isRelativeUrl = false;
    for (int i = 10; allData[i]; i++)
    {
        if (urlIndex == 20)
        {
            break;
        }
        // int mainUrlSize = size_tmy_strlen(url);
        if (charObj->charLowerCase(allData[i - 9]) == 'h' && charObj->charLowerCase(allData[i - 8]) == 'r' &&
            charObj->charLowerCase(allData[i - 7]) == 'e' && charObj->charLowerCase(allData[i - 6]) == 'f' &&
            allData[i - 5] == '=' && allData[i - 4] == '"' &&
            charObj->charLowerCase(allData[i - 3]) == 'h' && charObj->charLowerCase(allData[i - 2]) == 't' &&
            charObj->charLowerCase(allData[i - 1]) == 't' && charObj->charLowerCase(allData[i]) == 'p') // for http or https urls
        {
            startIndex = i - 3;
        }
        // else if (charLowerCase(allData[i - 5]) == 'h' && charLowerCase(allData[i - 4]) == 'r' &&
        //          charLowerCase(allData[i - 3]) == 'e' && charLowerCase(allData[i - 2]) == 'f' &&
        //          allData[i - 1] == '"' && (allData[i] == '.' || allData[i] == '/')) // for relative urls
        // {
        //     isRelativeUrl = true;
        //     startIndex = i;
        // }
        else if (startIndex != 0 && allData[i] == '"')
        {
            if ((allData[i - 1] == '/') ||
                (allData[i - 5] == '.' &&
                 charObj->charLowerCase(allData[i - 4]) == 'h' && charObj->charLowerCase(allData[i - 3]) == 't' && charObj->charLowerCase(allData[i - 2]) == 'm' && charObj->charLowerCase(allData[i - 1]) == 'l'))
            {
                if (i - startIndex > 0)
                {
                    // int newUrlSize = isRelativeUrl ? (mainUrlSize + (i - startIndex) + 1) : ((i - startIndex) + 1);
                    char *newUrl = new char[i - startIndex + 1];
                    int j = 0;
                    // if (isRelativeUrl)
                    // {
                    //     while (j < mainUrlSize)
                    //     {
                    //         newUrl[j] = url[j];
                    //         j++;
                    //     }
                    //     isRelativeUrl = false;
                    // }
                    while (startIndex < i)
                    {
                        newUrl[j++] = allData[startIndex++];
                    }
                    newUrl[j] = '\0';
                    thisPageUrls[urlIndex++] = newUrl;
                }
            }
            startIndex = 0;
        }
    }
    thisPageUrls[urlIndex++] = nullptr;
    return thisPageUrls;
}

// void clearArrayOfString(char **data)
// {
//     int i = 0;
//     while (data[i] != nullptr)
//     {
//         if (data[i])
//         {
//             delete[] data[i];
//         }
//         i++;
//     }
//     delete[] data;
//     data = nullptr;
// }

// void clearCharacters(char *data)
// {
//     delete[] data;
//     data = nullptr;
// }

// // Append src to dest
// void my_strcat(char *dest, const char *src)
// {
//     int i, sizeOfSrc = size_tmy_strlen(src), sizeOfDest = size_tmy_strlen(dest);
//     for (i = 0; i < sizeOfDest; i++)
//     {
//     }
//     for (int j = 0; j < sizeOfSrc; j++)
//     {
//         dest[i] = src[j];
//         i++;
//     }
//     dest[i] = '\0';
// }

// // convert character into lowercase
// char charLowerCase(char c)
// {
//     if (c >= 'A' && c <= 'Z')
//     {
//         return c + 32; // Convert to lowercase
//     }
//     return c;
// }

// // remove white spaces
// char *normalizeTextByRemovingSpaces(char *text)
// {
//     int i = 0, j = 0;
//     for (; text[i]; i++)
//     {
//         if (text[i] == '\t' || text[i] == ' ' || text[i] == '\n')
//         {
//             continue;
//         }
//         else
//         {
//             text[j] = text[i];
//             j++;
//         }
//     }
//     text[j] = '\0';
//     return text;
// }

// // return length before '\0'
// int size_tmy_strlen(const char *s)
// {
//     int count = 0;
//     while (s[count] != '\0')
//     {
//         count++;
//     }
//     return count;
// }

// // convert into lower case
// void lowercase(char *ch)
// {
//     if (!ch)
//         return;
//     int i = 0;
//     while (ch[i] != '\0')
//     {
//         if (ch[i] >= 'A' && ch[i] <= 'Z')
//         {
//             ch[i] += 32;
//         }
//         i++;
//     }
// }

// // convert long into string
// void longIntoString(long long num, char *str)
// {
//     str[0] = '/';
//     if (num == 0)
//     {
//         str[1] = '0';
//         str[2] = '\0';
//     }
//     else
//     {
//         int i = 1, rem = 0;
//         if (num < 0)
//         {
//             num = -num;
//             str[i] = '-';
//             i++;
//         }
//         long long revNum = 0;
//         while (num > 0)
//         {
//             rem = num % 10;
//             revNum = revNum * 10 + rem;
//             num /= 10;
//         }
//         while (revNum > 0)
//         {
//             str[i] = (revNum % 10) + '0';
//             revNum /= 10;
//             i++;
//         }
//         str[i] = '\0';
//     }
//     char extension[] = {'.', 'h', 't', 'm', 'l', '\0'};
//     my_strcat(str, extension);
// }

// // convert string into number
// long long stringIntoLong(char *str)
// {
//     int size = size_tmy_strlen(str), i = 0;
//     long long num = 0;
//     bool isNegative = false;
//     if (str[i] == '-')
//     {
//         isNegative = true;
//         i++;
//     }
//     for (; i < size; i++)
//     {
//         num = num * 10 + (str[i] - '0');
//     }
//     return num;
// }

// // Copy src into dest
// void my_strcpy(char *dest, const char *src)
// {
//     int i, sizeOfSrc = size_tmy_strlen(src);
//     for (i = 0; i < sizeOfSrc; i++)
//     {
//         dest[i] = src[i];
//     }
//     dest[i] = '\0';
// }

// // Substring search
// char *my_strstr(const char *haystack, const char *needle)
// {
//     char *newhaystack = new char[size_tmy_strlen(haystack) + 1];
//     my_strcpy(newhaystack, haystack);
//     char *newneedle = new char[size_tmy_strlen(haystack) + 1];
//     my_strcpy(newneedle, needle);
//     lowercase(newhaystack);
//     lowercase(newneedle);
//     int sizeOfNeedle = size_tmy_strlen(newneedle), sizeOfHayStack = size_tmy_strlen(newhaystack);
//     for (int i = 0; i < sizeOfHayStack; i++)
//     {
//         for (int j = 0; j < sizeOfNeedle; j++)
//         {
//             if (newhaystack[i] == newneedle[j])
//             {
//                 int compareLength = 0, traverI = i;
//                 for (int k = 0; k < sizeOfNeedle; k++)
//                 {
//                     if (newhaystack[traverI] == newneedle[k])
//                     {
//                         traverI++;
//                         compareLength++;
//                     }
//                     else
//                     {
//                         k = sizeOfNeedle;
//                     }
//                 }
//                 if (compareLength == sizeOfNeedle)
//                 {
//                     delete[] newhaystack;
//                     delete[] newneedle;
//                     return (char *)&haystack[i];
//                 }
//             }
//         }
//     }
//     delete[] newhaystack;
//     delete[] newneedle;
//     return nullptr;
// }