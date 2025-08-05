#ifndef HEADER_H
#define HEADER_H

#include <iostream>
using namespace std;

// function overloading for generice datatype for using hash function for different datatypes
int computeHash(int key, int size);
int computeHash(long key, int size);
int computeHash(float key, int size);
int computeHash(const string &key, int size);
int computeHash(const char *key, int size);

// function overloading for generice datatype for matching different datatypes
bool isMatched(int stored, int key);
bool isMatched(long stored, long key);
bool isMatched(float stored, float key);
bool isMatched(const string &stored, const string &key);
bool isMatched(const char *stored, const char *key);

template <typename keyType, typename valueType>
class Node // used for applying open chaining collision if happend
{
private:
    valueType val;
    keyType key;
    Node<keyType, valueType> *next;

public:
    // constructors
    Node(keyType key, valueType val);
    ~Node() {}

    // member functions
    keyType getKey();
    valueType getValue();
    Node<keyType, valueType> *getNext();
    void setNext(Node<keyType, valueType> *next);
    void setVal(valueType val);
};

template <typename keyType, typename valueType>
class HashMap
{
private:
    int size = 10, totalElements = 0;
    Node<keyType, valueType> **HashTable;

    void resizeHash();
    int hashFunction(keyType key);
    Node<keyType, valueType> *getPreviousNodeForKey(keyType key);

public:
    // constructors
    HashMap();
    ~HashMap();
    HashMap(HashMap &obj);

    // member functions
    void hashInsertion(keyType key, valueType val);
    void hashDeletion(keyType key);
    bool isPresent(keyType key);
    void hashDisplay();
    void clear();
    bool empty();
    int count();
    // void setValue(keyType key, valueType val);
    Node<keyType, valueType> *getNode(keyType key);

    // operator overloading for creating new obj
    HashMap<keyType, valueType> operator=(HashMap<keyType, valueType> *obj);
};

#include "./Hash library.cpp"

#endif