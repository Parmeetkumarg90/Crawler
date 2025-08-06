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
    delete charObj;
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
    if (!url || !path)
    {
        cout << "Please enter a url or path";
        return nullptr;
    }
    char *command = new char[charObj->size_tmy_strlen(url) + charObj->size_tmy_strlen(path) + 20](), *unqiueName = generateUniqueName(); //, *space = new char[2]{' ', '\0'};
    char urlPrefix[5] = "http";
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
    charObj->my_strcat(command, " ");
    charObj->my_strcat(command, path);
    // charObj->my_strcat(command, "/");
    charObj->my_strcat(command, unqiueName);
    charObj->my_strcat(command, " ");
    charObj->my_strcat(command, url);
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
        charObj->clearCharacters(unqiueName);
        charObj->clearCharacters(command);
        // charObj->clearCharacters(space);
        return nullptr;
    }
    char *newPath = new char[charObj->size_tmy_strlen(path) + charObj->size_tmy_strlen(unqiueName) + 2];
    charObj->my_strcpy(newPath, path);
    // charObj->my_strcat(newPath, "/");
    charObj->my_strcat(newPath, unqiueName);
    charObj->clearCharacters(unqiueName);
    charObj->clearCharacters(command);
    // charObj->clearCharacters(space);
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
    if (!thisPageUrl)
    {
        charObj->clearCharacters(allData);
        charObj->clearCharacters(currFilePath);
        return;
    }
    for (int i = 0; thisPageUrl[i] && i < maxFoundPerPage; i++)
    {
        char *urlCopy = new char[charObj->size_tmy_strlen(thisPageUrl[i]) + 1];
        charObj->my_strcpy(urlCopy, thisPageUrl[i]);
        allUrls->hashInsertion(urlCopy, 0);
        fileGetDfs(urlCopy, path, maxDepthCount - 1, maxFoundPerPage);
    }
    charObj->clearCharacters(allData);
    charObj->clearCharacters(currFilePath);
}

// find all urls
char **Crawler::readHtmlUrls(const char *allData, const char *url, int maxFoundPerPage)
{
    char **thisPageUrls = new char *[maxFoundPerPage + 1]();
    int startIndex = 0, urlIndex = 0;
    // bool isRelativeUrl = false;
    for (int i = 10; allData[i]; i++)
    {
        if (urlIndex + 1 == maxFoundPerPage)
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
                    int temp = startIndex;
                    while (temp < i)
                    {
                        newUrl[j++] = allData[temp++];
                    }

                    newUrl[j] = '\0';
                    thisPageUrls[urlIndex++] = newUrl;
                }
            }
            startIndex = 0;
        }
    }
    thisPageUrls[urlIndex] = nullptr;
    return thisPageUrls;
}