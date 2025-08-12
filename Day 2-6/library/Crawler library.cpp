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
    static const char *sw[] = {"a", "am", "an", "and", "are", "at", "because", "but", "by", "can", "could", "did", "do", "does", "for", "had", "has", "have", "how", "if", "in", "is", "may", "might", "must", "my", "of", "on", "or", "shall", "should", "the", "to", "was", "were", "what", "when", "where", "while", "who", "why", "will", "with", "would", "you", "your", nullptr};
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
        cout << "Path: " << path << unqiueName;
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
    if (!url || !path || maxDepthCount <= 0)
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
    else
    {
        allUrls->hashInsertion(url, 1);
    }
    char *currFilePath = wgetFileDownload(url, path);
    if (!currFilePath)
    {
        return;
    }
    // cout << currFilePath;
    // cout << "\n\n\nhere";
    // cout << "\n\n\n file download";
    char *allData = readFile(currFilePath);
    if (!allData)
    {
        charObj->clearCharacters(currFilePath);
        return;
    }
    // cout << "\n\n\n file read";
    // cout << allData;
    char *mostFrequentWord = most_frequent_word(allData, stopWords, stopWordCount);
    charObj->lowercase(mostFrequentWord);
    allData = charObj->normalizeTextByRemovingSpaces(allData);
    // cout << "\n\n\n file normalized";
    createLogFile(url, mostFrequentWord);
    char **thisPageUrl = readHtmlUrls(allData, url, maxFoundPerPage);
    if (!thisPageUrl)
    {
        charObj->clearCharacters(allData);
        charObj->clearCharacters(currFilePath);
        charObj->clearCharacters(mostFrequentWord);
        return;
    }
    // cout << "\n\n\n all urls read";
    for (int i = 0; thisPageUrl[i] && i < maxFoundPerPage; i++)
    {
        char *urlCopy = new char[charObj->size_tmy_strlen(thisPageUrl[i]) + 1]();
        charObj->my_strcpy(urlCopy, thisPageUrl[i]);
        allUrls->hashInsertion(urlCopy, 0);
        fileGetDfs(urlCopy, path, maxDepthCount - 1, maxFoundPerPage);
        charObj->clearCharacters(thisPageUrl[i]);
    }
    charObj->clearCharacters(allData);
    charObj->clearCharacters(currFilePath);
    charObj->clearCharacters(mostFrequentWord);
    charObj->clearArrayOfString(thisPageUrl);
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
        if (urlIndex >= maxFoundPerPage)
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
                char *dummyUrl = new char[(i - startIndex) + 1]();
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
                // if (isRelativeUrl)
                // {
                // if (charObj->endsWith(url, ".html"))
                // {
                //     if (charObj->my_strcmp(dummyUrl, "/") == 0 || charObj->my_strcmp(dummyUrl, "./") == 0)
                //     {
                //         charObj->clearCharacters(dummyUrl);
                //         charObj->clearCharacters(newUrl);
                //         continue;
                //     }
                // }
                // else
                // {
                //     if (charObj->my_strcmp(dummyUrl, "/") == 0 || charObj->my_strcmp(dummyUrl, "./") == 0)
                //     {
                //         charObj->clearCharacters(dummyUrl);
                //         charObj->clearCharacters(newUrl);
                //         continue;
                //     }
                //     charObj->my_strcat(newUrl, "/");
                //     charObj->my_strcat(newUrl, url);
                //     j += charObj->size_tmy_strlen(url);
                // }
                //     isRelativeUrl = false;
                // }
                if (isRelativeUrl)
                {
                    if (url[charObj->size_tmy_strlen(url) - 1] == '/')
                    {
                        charObj->my_strcat(newUrl, url);
                    }
                    else
                    {
                        charObj->my_strcat(newUrl, url);
                        charObj->my_strcat(newUrl, "/");
                    }
                    charObj->my_strcat(newUrl, dummyUrl);
                    isRelativeUrl = false;
                }
                else
                {
                    charObj->my_strcat(newUrl, dummyUrl);
                }
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

// checking if url is downloadable or not
bool Crawler::isUrlReachAble(const char *url)
{
    int urlSize = charObj->size_tmy_strlen(url);
    char *checkUrl = new char[urlSize + 40]();
    charObj->my_strcat(checkUrl, "wget --spider \"");
    charObj->my_strcat(checkUrl, url);
    charObj->my_strcat(checkUrl, "\" 2>/dev/null");
    // cout << "\n\n\n " << checkUrl << "\n\n\n";
    bool isReachable = system(checkUrl) == 0;
    charObj->clearCharacters(checkUrl);
    return isReachable;
}

// used for creating a log file
void Crawler::createLogFile(const char *url, char *mostFrequentWord)
{
    if (!url)
    {
        return;
    }
    char *allData = readFile("./pages/logFile.txt");
    if (!allData)
    {
        fstream file("./pages/logFile.txt", ios::app);
        if (!file.is_open())
        {
            cout << "Log File Creation Error";
            return;
        }
        file << "#" << mostFrequentWord << "->" << url;
        file << "\n\n";
        file.close();
    }
    else
    {
        int indexIfPresent = isKeywordPresentinFile(allData, mostFrequentWord);
        if (indexIfPresent == -1)
        {
            fstream file("./pages/logFile.txt", ios::app);
            if (!file.is_open())
            {
                cout << "Log File Opening Error(Keyword Not Present)";
                return;
            }
            file << "#" << mostFrequentWord << "->" << url << "," << "\n\n";
            file.close();
        }
        else
        {
            fstream file("./pages/logFile.txt", ios::out | ios::trunc);
            if (!file.is_open())
            {
                cout << "Log File Opening Error(Keyword Present)";
                return;
            }
            int dataSize = charObj->size_tmy_strlen(allData), urlSize = charObj->size_tmy_strlen(url);
            char *newAllData = new char[dataSize + urlSize + 10]();
            int i = 0, j = 0, urlI = 0;
            for (i = 0; i < indexIfPresent; i++) // copy all data before keyword
            {
                newAllData[j++] = allData[i];
            }
            for (; i < dataSize && allData[i] != '>'; i++) // copy all data before >
            {
                newAllData[j++] = allData[i];
            }
            newAllData[j++] = allData[i++];        // copy >
            for (urlI = 0; urlI < urlSize; urlI++) // add url
            {
                newAllData[j++] = url[urlI];
            }
            newAllData[j++] = ',';    // add ","
            for (; i < dataSize; i++) // add remaining portion
            {
                newAllData[j++] = allData[i];
            }
            file << newAllData;
            file.close();
            charObj->clearCharacters(newAllData);
        }
        charObj->clearCharacters(allData);
    }
}

// return index from where it matched the keyword otherwise -1
int Crawler::isKeywordPresentinFile(const char *allData, const char *keyword)
{
    for (int i = 0; allData[i] && allData[i] != '\0'; i++)
    {
        if (allData[i] == '#')
        {
            if (charObj->startsWith(&allData[i + 1], keyword))
            {
                return i + 1;
            }
        }
    }
    return -1;
}

// Most frequently used word(ignoring stopwords)
char *Crawler::most_frequent_word(char *text, const char **stopwords, int stopcount)
{
    if (!text)
    {
        return nullptr;
    }
    char *frequentWord = new char[20]();
    char *traverseWord = new char[20]();
    int i = 0, startI = 0, endI = 0, maxCount = 0;
    char *newText = removeTags(text);
    HashMap<char *, int> *obj = new HashMap<char *, int>();
    while (newText[i])
    {
        if ((charObj->charLowerCase(newText[i]) >= 97 && charObj->charLowerCase(newText[i]) <= 122) || (charObj->charLowerCase(newText[i]) >= 48 && charObj->charLowerCase(newText[i]) <= 57))
        {
            if (endI < 19)
            {
                traverseWord[endI] = newText[i];
                endI++;
                traverseWord[endI] = '\0';
            }
        }
        else
        {
            if (endI > 1)
            {
                traverseWord[endI] = '\0';
                bool isStop = charObj->findWordInArrayOfChar(traverseWord, stopwords);
                if (!isStop)
                {
                    Node<char *, int> *node = obj->getNode(traverseWord);
                    if (!node)
                    {
                        char *keyword = new char[charObj->size_tmy_strlen(traverseWord) + 1]();
                        charObj->my_strcpy(keyword, traverseWord);
                        obj->hashInsertion(keyword, 1);
                        if (maxCount < 1)
                        {
                            maxCount = 1;
                            charObj->my_strcpy(frequentWord, keyword);
                        }
                    }
                    else
                    {
                        int count = node->getValue() + 1;
                        node->setVal(count);
                        if (maxCount < count)
                        {
                            maxCount = count;
                            charObj->my_strcpy(frequentWord, node->getKey());
                        }
                    }
                }
                endI = 0;
            }
        }
        i++;
    }
    charObj->clearCharacters(traverseWord);
    charObj->clearCharacters(newText);
    // obj->hashDisplay();
    // auto node = obj->getNode(frequentWord);
    // if (node)
    // {
    //     cout << "\t " << node->getKey() << "\t " << node->getValue();
    // }
    // cout << "\t " << obj->getNode(frequentWord)->getKey() << "\t " << obj->getNode(frequentWord)->getValue();
    delete obj;
    // cout << "\n\n " << frequentWord << "\n\n";
    return frequentWord;
}

// remove all html tags
char *Crawler::removeTags(const char *text)
{
    char *newText = new char[charObj->size_tmy_strlen(text) + 1]();
    bool inTag = false, isCss = false, isScript = false;
    int currI = 0;
    for (int i = 0; text[i]; i++)
    {
        if (text[i] == '<')
        {
            inTag = true;
            if (charObj->startsWith(&text[i], "<style"))
            {
                isCss = true;
            }
            else if (charObj->startsWith(&text[i], "</style>"))
            {
                isCss = false;
            }
            if (charObj->startsWith(&text[i], "<script"))
            {
                isScript = true;
            }
            else if (charObj->startsWith(&text[i], "</script>"))
            {
                isScript = false;
            }
        }
        else if (text[i] == '>')
        {
            inTag = false;
        }
        else if (!inTag && !isCss && !isScript)
        {
            newText[currI++] = text[i];
        }
    }
    newText[currI] = '\0';
    // cout << newText;
    return newText;
}