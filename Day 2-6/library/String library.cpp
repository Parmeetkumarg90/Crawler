#include <iostream>
#include <fstream>
#include "./HashMap(generic).h"
#include "./String.h"

using namespace std;

// Task 1

int Character::size_tmy_strlen(const char *s) // return length before '\0'
{
    int count = 0;
    while (s[count] != '\0')
    {
        count++;
    }
    return count;
}

int Character::my_strcmp(const char *a, const char *b) // lexicographical compare
{
    int sizeOfA = size_tmy_strlen(a), sizeOfB = size_tmy_strlen(b);
    char ch1, ch2;
    for (int i = 0, j = 0; i < sizeOfA && j < sizeOfB; i++, j++)
    {
        ch1 = a[i];
        ch2 = b[i];
        if (ch1 >= 'A' && ch1 <= 'Z')
        {
            ch1 += 32;
        }
        if (ch2 >= 'A' && ch2 <= 'Z')
        {
            ch2 += 32;
        }
        if (ch1 != ch2)
        {
            return a[i] - b[i];
        }
    }
    return 0;
}

void Character::lowercase(char *ch)
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
void Character::my_strcpy(char *dest, const char *src)
{
    int i, sizeOfSrc = size_tmy_strlen(src);
    for (i = 0; i < sizeOfSrc; i++)
    {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

// Append src to dest
void Character::my_strcat(char *dest, const char *src)
{
    int i = 0;
    while (dest[i] != '\0')
    {
        i++;
    }
    int j = 0;
    while (src[j] != '\0')
    {
        dest[i] = src[j];
        i++;
        j++;
    }
    dest[i] = '\0';
}

// Find first occurrence of char
char *Character::my_strchr(const char *s, int c)
{
    int sizeOfS = size_tmy_strlen(s);
    for (int i = 0; i < sizeOfS; i++)
    {
        if (s[i] == (char)c)
        {
            return (char *)&s[i];
        }
    }
    return nullptr;
}

// Reverse a string in place
void Character::reverse_inplace(char *s)
{
    int sizeOfS = size_tmy_strlen(s) - 1;
    char temp;
    for (int i = 0; i < sizeOfS; i++)
    {
        temp = s[i];
        s[i] = s[sizeOfS];
        s[sizeOfS] = temp;
        sizeOfS--;
    }
}

// Check if palindrome
int Character::is_palindrome(const char *s)
{
    int sizeOfS = size_tmy_strlen(s) - 1;
    for (int i = 0; i < sizeOfS; i++)
    {
        if (s[i] != s[sizeOfS])
        {
            return 0;
        }
        sizeOfS--;
    }
    return 1;
}

// Count words
int Character::count_words(const char *s)
{
    int totalWord = 0, sizeOfS = size_tmy_strlen(s);
    bool isWord = false;
    for (int i = 0; i < sizeOfS; i++)
    {
        if ((s[i] >= 97 && s[i] <= 122) || (s[i] >= 65 && s[i] <= 90))
        {
            if (isWord)
            {
                totalWord++;
            }
            isWord = false;
        }
        else
        {
            isWord = true;
        }
    }
    return totalWord;
}

// Most frequently used word(ignoring stopwords)
char **Character::most_frequent_word(const char *text, const char **stopwords, int stopcount)
{
    if (!text)
    {
        return nullptr;
    }
    char **allWords = tokenizer(text);
    if (!allWords || !allWords[0])
    {
        return nullptr;
    }
    HashMap<char *, int> *obj = new HashMap<char *, int>();
    char **frequentWords = new char *[10]();
    int wordI = 0;
    for (int i = 0; allWords[i]; i++)
    {
        bool isStopWord = findWordInArrayOfChar(allWords[i], stopwords);
        if (isStopWord)
        {
            continue;
        }
        Node<char *, int> *node = obj->getNode(allWords[i]);
        if (node)
        {
            frequentWords[wordI] = node->getKey();
            node->setVal(node->getValue() + 1);
            wordI++;
            if (wordI >= 10)
            {
                break;
            }
        }
        else if (!obj->isPresent(allWords[i]))
        {
            obj->hashInsertion(allWords[i], 1);
        }
        node = nullptr;
    }
    delete obj;
    obj = nullptr;
    return frequentWords;
}

bool Character::findWordInArrayOfChar(const char *ch, const char **list)
{
    for (int j = 0; list[j]; j++)
    {
        if (my_strcmp(ch, list[j]) == 0)
        {
            return true;
        }
    }
    return false;
}

// Task 2
char **Character::tokenizer(const char *data)
{
    if (!data)
    {
        return nullptr;
    }
    int wordI = 0, fieldI = 0, i;
    char **allFields = new char *[20000]();
    char *eachWord = new char[15]();
    bool isWord = false;
    for (i = 0; data[i] != '\0'; i++)
    {
        if (wordI >= 14 || fieldI >= 19998)
        {
            break;
        }
        if (data[i] == ' ' || data[i] == ',' || data[i] == '\n')
        {
            if (isWord)
            {
                allFields[fieldI] = new char[wordI + 1];
                for (int j = 0; j < wordI; j++)
                {
                    allFields[fieldI][j] = eachWord[j];
                }
                allFields[fieldI][wordI] = '\0';
                fieldI++;
                wordI = 0;
            }
            isWord = false;
        }
        else
        {
            eachWord[wordI] = data[i];
            wordI++;
            eachWord[wordI] = '\0';
            isWord = true;
        }
    }
    if (isWord)
    {
        allFields[fieldI] = new char[wordI + 1];
        for (int j = 0; j < wordI; j++)
        {
            allFields[fieldI][j] = eachWord[j];
        }
        allFields[fieldI][wordI] = '\0';
        fieldI++;
    }

    allFields[fieldI] = nullptr;
    delete[] eachWord;
    return allFields;
}

char *Character::readFile(const char *filePath)
{
    fstream file(filePath, ios::in);
    if (!file.is_open())
    {
        return nullptr;
    }
    int i = 0;
    char ch, *allData = new char[10000000]();
    while (file.get(ch))
    {
        if (i < 9999999)
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

void Character::clearArrayOfString(char **&data)
{
    if (data)
    {
        int i = 0;
        while (data[i] != nullptr)
        {
            if (data[i])
            {
                delete[] data[i];
            }
            i++;
        }
        delete[] data;
    }
    data = nullptr;
}

void Character::clearCharacters(char *&data)
{
    if (data)
    {
        delete[] data;
    }
    data = nullptr;
}

// Task 3
char *Character::fillPlaceholder(char *text, HashMap<char *, char *> *obj)
{
    if (!text || !text[0] || !obj)
    {
        cout << "Text or object missing";
        return nullptr;
    }
    char **allPlaceholders = new char *[10], *eachWord = new char[20];
    bool isParenthesisOpen = false;
    int eachWordI = 0, placeIndex = 0;
    int indexOfEachKey[10];

    // get all placeholders
    for (int i = 1; text[i]; i++)
    {
        if (text[i - 1] == '{' && text[i] == '{')
        {
            eachWordI = 0;
            isParenthesisOpen = true;
            indexOfEachKey[placeIndex] = i + 1;
        }
        else if ((text[i + 1] == '}' && text[i] == '}'))
        {
            allPlaceholders[placeIndex] = new char[eachWordI + 1];
            for (int j = 0; j < eachWordI; j++)
            {
                allPlaceholders[placeIndex][j] = eachWord[j];
            }
            allPlaceholders[placeIndex][eachWordI] = '\0';
            placeIndex++;
            eachWordI = 0;
            isParenthesisOpen = false;
        }
        else if (isParenthesisOpen)
        {
            eachWord[eachWordI] = text[i];
            eachWordI++;
            eachWord[eachWordI] = '\0';
        }
    }
    delete[] eachWord;
    eachWord = nullptr;
    int currKey = 0, newTextI = 0;
    char *newText = new char[size_tmy_strlen(text) * 2]();

    // insert all values of placeholders
    for (int i = 0; text[i]; i++)
    {
        if (indexOfEachKey[currKey] - 2 > i)
        {
            newText[newTextI] = text[i];
            newTextI++;
        }
        else if (indexOfEachKey[currKey] - 2 == i)
        {
            Node<char *, char *> *currNode = obj->getNode(allPlaceholders[currKey]);
            if (!currNode)
            {
                break;
            }
            char *eachValue = currNode->getValue();
            for (int j = 0; eachValue[j]; j++)
            {
                newText[newTextI] = eachValue[j];
                newTextI++;
            }
            i += size_tmy_strlen(allPlaceholders[currKey]) + 3;
            currKey++;
        }
        newText[newTextI] = '\0';
    }
    clearArrayOfString(allPlaceholders);
    return newText;
}

// Task 4
// Substring search
char *Character::my_strstr(const char *haystack, const char *needle)
{
    char *newhaystack = new char[size_tmy_strlen(haystack) + 1];
    my_strcpy(newhaystack, haystack);
    char *newneedle = new char[size_tmy_strlen(needle) + 1];
    my_strcpy(newneedle, needle);
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

// Task 5
// remove extra white extra spaces
char *Character::normalizeTextByRemovingExtraSpace(char *text)
{
    int i = 1, j = 0;
    for (; text[i]; i++)
    {
        if ((text[i - 1] == '\t' && text[i] == '\t') || (text[i - 1] == ' ' && text[i] == ' ') || (text[i - 1] == '\n' && text[i] == '\n'))
        {
            continue;
        }
        else
        {
            text[j] = text[i];
            j++;
        }
    }
    text[j] = '\0';
    return text;
}

// convert character into lowercase
char Character::charLowerCase(char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return c + 32; // Convert to lowercase
    }
    return c;
}

// remove white spaces
char *Character::normalizeTextByRemovingSpaces(char *text)
{
    int i = 0, j = 0;
    for (; text[i]; i++)
    {
        if (text[i] == '\t' || text[i] == ' ' || text[i] == '\n')
        {
            continue;
        }
        else
        {
            text[j] = text[i];
            j++;
        }
    }
    text[j] = '\0';
    return text;
}

// convert long into string
void Character::longIntoString(long long num, char *str)
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
long long Character::stringIntoLong(char *str)
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

// check if a mainStr ends with searchStr
bool Character::endsWith(const char *mainStr, const char *searchStr)
{
    if (!mainStr || !searchStr)
    {
        return false;
    }
    int j = size_tmy_strlen(searchStr) - 1, i = size_tmy_strlen(mainStr) - 1;
    if (j >= i)
    {
        return false;
    }
    while (j >= 0)
    {
        if (!mainStr[i] || !searchStr[j] || (charLowerCase(mainStr[i]) != charLowerCase(searchStr[j])))
        {
            return false;
        }
        i--;
        j--;
    }
    return true;
}

// check if a mainStr starts with searchStr
bool Character::startsWith(const char *mainStr, const char *searchStr)
{
    if (!mainStr || !searchStr)
    {
        return false;
    }
    int j = 0, i = 0, mainLen = size_tmy_strlen(mainStr), searchLen = size_tmy_strlen(searchStr);
    if (searchLen > mainLen)
    {
        return false;
    }
    while (j < searchLen)
    {
        if (!mainStr[i] || !searchStr[j] || (charLowerCase(mainStr[i]) != charLowerCase(searchStr[j])))
        {
            return false;
        }
        i++;
        j++;
    }
    return true;
}
char *Character::findExtension(const char *str)
{
    if (!str)
        return nullptr;

    int len = size_tmy_strlen(str);
    int i = len - 1;

    while (i >= 0 && str[i] != '.')
    {
        if (str[i] == '?')
        {
            break;
        }
        i--;
    }

    if (i < 0 || i == len - 1)
    {
        return nullptr;
    }
    char *ext = new char[len - i]();
    int j = 0;
    for (int k = i + 1; k < len; k++)
    {
        ext[j++] = str[k];
    }
    ext[j] = '\0';
    return ext;
}
