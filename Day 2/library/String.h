#ifndef String_H
#define String_H

class Character
{
public:
    // Task 1
    int size_tmy_strlen(const char *s);          // return length before '\0'
    int my_strcmp(const char *a, const char *b); // lexicographical compare
    void my_strcpy(char *dest, const char *src); // copy src into dest
    // char *my_strcpy(char *dest, const char *src);                                       // copy src into dest
    void my_strcat(char *dest, const char *src);                                        // append src to dest
    char *my_strchr(const char *s, int c);                                              // find first occurrence of char
    void reverse_inplace(char *s);                                                      // reverse a string
    int is_palindrome(const char *s);                                                   // check if palidrome
    int count_words(const char *s);                                                     // count words
    void lowercase(char *ch);                                                           // convert into lowercase
    char charLowerCase(char c);                                                         // convert character into lowercase
    bool findWordInArrayOfChar(const char *ch, const char **list);                      // find if a word is present in a list of words
    char **most_frequent_word(const char *text, const char **stopwords, int stopcount); // most frequestly used words

    // Task 2
    char *readFile(const char *filePath); // read a file
    char **tokenizer(const char *data);   // return all fields
    void clearArrayOfString(char **data); // clear a array of character string
    void clearCharacters(char *data);     // clear a array of characters

    // // Task 3
    char *fillPlaceholder(char *text, HashMap<char *, char *> *obj);

    // // Task 4
    char *my_strstr(const char *haystack, const char *needle); // substring search

    // Task 5
    char *normalizeTextByRemovingExtraSpace(char *text); // remove extra white spaces

    // Basic or normally used functions
    char *normalizeTextByRemovingSpaces(char *text); // remove white spaces
    long long stringIntoLong(char *str);             // convert string into number
    void longIntoString(long long num, char *str);   // convert long into string
};

#include "./String library.cpp"

#endif