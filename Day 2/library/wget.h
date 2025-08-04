void my_strcat(char *dest, const char *src);               // concat
int size_tmy_strlen(const char *s);                        // find length
char *my_strstr(const char *haystack, const char *needle); // substring search
void lowercase(const char *ch);                            // convert in lowercase
char *my_strcpy(char *dest, const char *src);              // copy src into dest

bool isDirectoryPresent(const char *dirPath);  // is directory present
void makeDIrectory(const char *dirPath);       // create a directory
char *generateUniqueName();                    // generate a unique name
void longIntoString(long long num, char *str); // convert long into string

#include "./library.cpp"