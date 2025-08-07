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
    static const char *sw[] = {"a", "am", "an", "and", "are", "at", "because", "but", "by", "can", "could", "did", "do", "does", "for", "had", "has", "have", "how", "if", "in", "is", "may", "might", "must", "my", "of", "on", "or", "shall", "should", "the", "to", "was", "were", "what", "when", "where", "while", "who", "why", "will", "with", "would", "you", "your", "html", "p", "nav", "div", "class", "id", "style", "<", ">", nullptr};
    static const char *na[] = {".aac", ".aax", ".ai", ".aiff", ".alac", ".ape", ".au", ".avi", ".avif", ".bmp", ".bpg", ".css", ".dss", ".eot", ".eps", ".f4v", ".flac", ".flv", ".gif", ".gsm", ".heic", ".heif", ".ico", ".java", ".jpeg", ".jpg", ".js", ".json", ".jsx", ".less", ".m4a", ".m4v", ".mkv", ".mov", ".mp3", ".mp4", ".mpc", ".mpeg", ".mpg", ".ogg", ".otf", ".pdf", ".png", ".psd", ".py", ".raw", ".sass", ".scss", ".svg", ".tif", ".tiff", ".ts", ".tsx", ".ttf", ".wav", ".webm", ".webp", ".wma", ".wmv", ".woff", ".woff2", nullptr};
    stopWordCount = sizeof(sw) / sizeof(sw[0]);
    notAllowedCount = sizeof(na) / sizeof(na[0]);
    stopWords = sw;
    notAllowed = na;
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
        cout << "Failed to create directory or directory present";
    }
}

// generate a unique name
char *Crawler::generateUniqueName()
{
    char *uniqueName = new char[30]();
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
    char *command = new char[charObj->size_tmy_strlen(url) + charObj->size_tmy_strlen(path) + 40](), *unqiueName = generateUniqueName(); //, *space = new char[2]{' ', '\0'};
    char urlPrefix[5] = "http";
    char *isFound = charObj->my_strstr(url, urlPrefix);
    if (!isFound)
    {
        cout << "\nUrl is invalid: " << url;
        charObj->clearCharacters(command);
        charObj->clearCharacters(unqiueName);
        return nullptr;
    }
    if (!path)
    {
        cout << "\nPlease enter a path";
        charObj->clearCharacters(command);
        charObj->clearCharacters(unqiueName);
        return nullptr;
    }
    // cout << "\n\n\nBefore path: " << path << unqiueName;
    bool isDir = isDirectoryPresent(path);
    if (!isDir)
    {
        makeDIrectory(path);
    }
    // cout << "\n\n\nAfter path: " << path << unqiueName;
    charObj->my_strcat(command, "wget -q -O");
    charObj->my_strcat(command, " ");
    charObj->my_strcat(command, path);
    charObj->my_strcat(command, unqiueName);
    charObj->my_strcat(command, " \"");
    charObj->my_strcat(command, url);
    charObj->my_strcat(command, "\"");
    // cout << "\n\n\n " << command;
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
    // cout << "\n\n\n download file";
    char *newPath = new char[charObj->size_tmy_strlen(path) + charObj->size_tmy_strlen(unqiueName) + 2]();
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
    char ch, *allData = new char[100000]();
    while (file.get(ch))
    {
        if (i < 99990)
        {
            allData[i] = ch;
        }
        else
        {
            break;
        }
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
    // cout << "\n\n\nhere";
    // cout << "\n\n\n " << url << "\n\n\n";
    char *allData = readFile(currFilePath);
    // cout << "\n\n\nhere";
    if (!allData)
    {
        charObj->clearCharacters(currFilePath);
        return;
    }
    // cout << allData;
    allData = charObj->normalizeTextByRemovingSpaces(allData);
    char **mostFrequentWords = charObj->most_frequent_word(allData, stopWords, stopWordCount);
    createLogFile(url, currFilePath, mostFrequentWords);
    for (int i = 0; mostFrequentWords[i]; i++)
    {
        cout << mostFrequentWords[i] << " ";
    }
    char **thisPageUrl = readHtmlUrls(allData, url, maxFoundPerPage);
    if (!thisPageUrl)
    {
        charObj->clearCharacters(allData);
        charObj->clearCharacters(currFilePath);
        charObj->clearArrayOfString(mostFrequentWords);
        return;
    }
    for (int i = 0; thisPageUrl[i] && i < maxFoundPerPage; i++)
    {
        char *urlCopy = new char[charObj->size_tmy_strlen(thisPageUrl[i]) + 1]();
        charObj->my_strcpy(urlCopy, thisPageUrl[i]);
        allUrls->hashInsertion(urlCopy, 0);
        fileGetDfs(urlCopy, path, maxDepthCount - 1, maxFoundPerPage);
        // charObj->clearCharacters(urlCopy); // remove
    }
    charObj->clearCharacters(allData);
    charObj->clearCharacters(currFilePath);
    charObj->clearArrayOfString(mostFrequentWords);
    // charObj->clearArrayOfString(thisPageUrl); // remove
}

// find all urls
char **Crawler::readHtmlUrls(const char *allData, const char *url, int maxFoundPerPage)
{
    char **thisPageUrls = new char *[maxFoundPerPage + 2]();
    int startIndex = 0, urlIndex = 0;
    bool isRelativeUrl = false;
    int mainUrlSize = charObj->size_tmy_strlen(url);
    for (int i = 10; allData[i]; i++)
    {
        if (i < 9)
        {
            continue;
        }
        if (urlIndex + 1 == maxFoundPerPage)
        {
            break;
        }
        if (charObj->charLowerCase(allData[i - 9]) == 'h' && charObj->charLowerCase(allData[i - 8]) == 'r' &&
            charObj->charLowerCase(allData[i - 7]) == 'e' && charObj->charLowerCase(allData[i - 6]) == 'f' &&
            allData[i - 5] == '=' && allData[i - 4] == '"'
            // &&
            // charObj->charLowerCase(allData[i - 3]) == 'h' && charObj->charLowerCase(allData[i - 2]) == 't' &&
            // charObj->charLowerCase(allData[i - 1]) == 't' && charObj->charLowerCase(allData[i]) == 'p'
            ) // for http or https urls
        {
            if (allData[i - 3] == '/' || allData[i - 3] == '.')
            {
                isRelativeUrl = true;
            }
            startIndex = i - 3;
        }
        else if (startIndex != 0 && allData[i] == '"')
        {
            if (i - startIndex > 0)
            {
                int newUrlSize = isRelativeUrl ? (mainUrlSize + (i - startIndex) + 1) : ((i - startIndex) + 1);
                char *newUrl = new char[newUrlSize + 5]();
                char *dummyUrl = new char[(i - startIndex) + 5]();
                int j = 0, temp = startIndex;
                while (temp < i)
                {
                    dummyUrl[j++] = allData[temp++];
                }
                dummyUrl[j] = '\0';
                const char *ext = charObj->findExtension(dummyUrl);
                if (ext && charObj->findWordInArrayOfChar(ext, notAllowed))
                {
                    charObj->clearCharacters(dummyUrl);
                    charObj->clearCharacters(newUrl);
                    continue;
                }
                if (isRelativeUrl)
                {
                    if (charObj->endsWith(url, ".html"))
                    {
                        if (charObj->my_strcmp(dummyUrl, "/") == 0 || charObj->my_strcmp(dummyUrl, "./") == 0)
                        {
                            charObj->clearCharacters(dummyUrl);
                        }
                    }
                    else
                    {
                        charObj->my_strcat(newUrl, "/");
                        charObj->my_strcat(newUrl, url);
                        j += charObj->size_tmy_strlen(url);
                    }
                    isRelativeUrl = false;
                }
                charObj->my_strcat(newUrl, dummyUrl);
                charObj->clearCharacters(dummyUrl);
                if (charObj->startsWith(newUrl, "http") && isUrlReachAble(newUrl))
                {
                    // if ()
                    // {
                    // cout << "\n\n\n " << newUrl << "\n\n\n";
                    thisPageUrls[urlIndex++] = newUrl;
                    // }
                }
                else
                {
                    charObj->clearCharacters(newUrl);
                }
            }
            startIndex = 0;
        }
    }
    thisPageUrls[urlIndex] = nullptr;
    return thisPageUrls;
}

bool Crawler::isUrlReachAble(const char *url)
{
    int urlSize = charObj->size_tmy_strlen(url);
    char *checkUrl = new char[urlSize + 40]();
    charObj->my_strcat(checkUrl, "wget --spider \"");
    charObj->my_strcat(checkUrl, url);
    charObj->my_strcat(checkUrl, "\" 2>/dev/null");
    // cout << "\n\n\n " << checkUrl << "\n\n\n";
    return system(checkUrl) == 0;
}

void Crawler::createLogFile(const char *url, const char *filePath, char **mostFrequentWords)
{
    if (!url || !filePath)
    {
        return;
    }
    fstream file("./pages/logFile.txt", ios::app);
    if (!file.is_open())
    {
        cout << "Log File Created";
        return;
    }
    file << "\n";
    file << url << "->" << filePath << "->";
    for (int i = 0; mostFrequentWords[i]; i++)
    {
        file << mostFrequentWords[i] << ",";
    }
    file.close();
}