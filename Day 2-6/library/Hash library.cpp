#include <iostream>
#include <type_traits>
#include "./HashMap(generic).h"
using namespace std;

template <typename keyType, typename valueType>
Node<keyType, valueType>::Node(keyType key, valueType value)
{ // constructor for initialization of node
    this->val = value;
    this->key = key;
    this->next = nullptr;
}

template <typename keyType, typename valueType>
keyType Node<keyType, valueType>::getKey()
{
    return key;
}

template <typename keyType, typename valueType>
valueType Node<keyType, valueType>::getValue()
{
    return val;
}

template <typename keyType, typename valueType>
Node<keyType, valueType> *Node<keyType, valueType>::getNext()
{
    return next;
}

template <typename keyType, typename valueType>
void Node<keyType, valueType>::setNext(Node<keyType, valueType> *next)
{
    this->next = next;
}
template <typename keyType, typename valueType>
void Node<keyType, valueType>::setVal(valueType val)
{
    this->val = val;
}

template <typename keyType, typename valueType>
HashMap<keyType, valueType>::HashMap()
{
    size = 5;
    HashTable = new Node<keyType, valueType> *[size]();
}

template <typename keyType, typename valueType>
HashMap<keyType, valueType>::HashMap(HashMap &obj)
{
    this->size = obj.size;
    this->totalElements = obj.totalElements;
    this->HashTable = new Node<keyType, valueType> *[size]();
    for (int i = 0; i < obj.size; i++)
    {
        Node<keyType, valueType> *objEachHead = obj.HashTable[i];
        while (objEachHead)
        {
            this->hashInsertion(objEachHead->getKey(), objEachHead->getValue());
            objEachHead = objEachHead->getNext();
        }
    }
}

template <typename keyType, typename valueType>
HashMap<keyType, valueType>::~HashMap()
{
    clear();
    if (HashTable)
    {
        delete[] HashTable;
    }
    HashTable = nullptr;
}

template <typename keyType, typename valueType>
void HashMap<keyType, valueType>::hashInsertion(keyType key, valueType val)
{
    int headPosition = hashFunction(key);
    Node<keyType, valueType> *headNode = HashTable[headPosition];

    Node<keyType, valueType> *current = headNode;
    while (current)
    {
        if (isMatched(current->getKey(), key))
        {
            // cout << "\nKey-Value pair {" << key << "," << val << "} already exists";
            return;
        }
        current = current->getNext();
    }

    Node<keyType, valueType> *newNode = new Node<keyType, valueType>(key, val);
    newNode->setNext(headNode);
    HashTable[headPosition] = newNode;

    // cout << "\nKey-Value pair {" << key << "," << val << "} added Successfully";
    totalElements++;

    if (((totalElements * 100) / size) >= 75)
    {
        resizeHash();
    }
}

template <typename keyType, typename valueType>
void HashMap<keyType, valueType>::hashDeletion(keyType key)
{
    int headPosition = hashFunction(key);
    Node<keyType, valueType> *headNode = HashTable[headPosition];
    if (!headNode) // not a single key-pair at position
    {
        cout << "\nHashTable is empty for this key";
        return;
    }
    Node<keyType, valueType> *before = getPreviousNodeForKey(key);
    if (before)
    {
        Node<keyType, valueType> *deletingNode = before->getNext();
        if (!deletingNode) // not found and currently at last node
        {
            cout << "Key-Value pair is not present in HashTable to delete";
        }
        else if (deletingNode && deletingNode->getKey() == key) // found pair
        {
            before->setNext(deletingNode->getNext());
            deletingNode->setNext(nullptr);
            // cout << "Key-value pair {" << deletingNode->getKey() << "," << deletingNode->getValue() << "} deleted successfully";
            totalElements--;
            delete deletingNode;
        }
    }
    else if (headNode && headNode->getKey() == key) // found at head
    {
        HashTable[headPosition] = headNode->getNext() ? headNode->getNext() : nullptr;
        headNode->setNext(nullptr);
        // cout << "Key-value pair {" << headNode->getKey() << "," << headNode->getValue() << "} deleted successfully";
        totalElements--;
        delete headNode;
    }
    else // not found
    {
        cout << "Key-Value pair is not present in HashTable to delete";
    }
}

template <typename keyType, typename valueType>
Node<keyType, valueType> *HashMap<keyType, valueType>::getPreviousNodeForKey(keyType key)
{
    int headPosition = hashFunction(key);
    Node<keyType, valueType> *prev = nullptr;
    if (!HashTable[headPosition])
    {
        cout << "\nHashTable is empty for this key";
    }
    else
    {
        Node<keyType, valueType> *head = HashTable[headPosition];
        while (head)
        {
            if (isMatched(head->getKey(), key)) // key found
            {
                break;
            }
            prev = head;
            head = head->getNext();
        }
    }
    return prev;
}

template <typename keyType, typename valueType>
void HashMap<keyType, valueType>::hashDisplay() // display all pairs from the hashtable bucket
{
    cout << "{";
    Node<keyType, valueType> *traverseEach = nullptr;
    for (int i = 0; i < size; i++)
    {
        traverseEach = HashTable[i]; // find all pairs at each location in map
        if (traverseEach)
        {
            while (traverseEach)
            {
                cout << "\n    {" << traverseEach->getKey() << "," << traverseEach->getValue() << "},";
                traverseEach = traverseEach->getNext();
            }
        }
    }
    cout << "\n}";
}

template <typename keyType, typename valueType>
void HashMap<keyType, valueType>::clear() // clear the whole hashtable
{
    Node<keyType, valueType> *traverse = nullptr, *deleteNode = nullptr;
    if (!HashTable)
    {
        cout << "\nHashTable is already empty";
        return;
    }
    for (int i = 0; i < size; i++)
    {
        traverse = HashTable[i];
        while (traverse)
        {
            deleteNode = traverse;
            traverse = traverse->getNext();
            if constexpr (is_same<char *, valueType>::value)
            {
                delete[] traverse->getKey();
            }
            if constexpr (is_same<keyType, char *>::value)
            {
                delete[] traverse->getValue();
            }
            deleteNode->setNext(nullptr);
            delete deleteNode;
        }
        HashTable[i] = nullptr;
    }
    totalElements = 0;
    // cout << "\nHashTable Cleared Successfully\n";
}

template <typename keyType, typename valueType>
int HashMap<keyType, valueType>::count() // count all pairs present in hashtable
{
    // int totalNodes = 0;
    // Node<keyType, valueType> *traverse = nullptr;
    // for (int i = 0; i < size; i++)
    // {
    //     traverse = HashTable[i];
    //     while (traverse)
    //     {
    //         totalNodes++;
    //         traverse = traverse->getNext();
    //     }
    // }
    // return totalNodes;
    return totalElements;
}

template <typename keyType, typename valueType>
bool HashMap<keyType, valueType>::empty() // check if whole hashtable is empty or not
{
    // bool isEmpty = true;
    // Node<keyType, valueType> *traverse = nullptr;
    // for (int i = 0; i < size; i++)
    // {
    //     traverse = HashTable[i];
    //     if (traverse)
    //     {
    //         return false;
    //     }
    // }
    // return isEmpty;
    return totalElements == 0;
}

template <typename keyType, typename valueType>
bool HashMap<keyType, valueType>::isPresent(keyType key)
{
    int headPosition = hashFunction(key);
    Node<keyType, valueType> *headNode = HashTable[headPosition];
    if (headNode)
    {
        Node<keyType, valueType> *before = getPreviousNodeForKey(key);
        if (before)
        {
            Node<keyType, valueType> *matchedNode = before->getNext();
            if (matchedNode) // found pair
            {
                return true;
            }
        }
        else if (headNode->getKey() == key) // found at head
        {
            return true;
        }
    }
    return false;
}

template <typename keyType, typename valueType>
void HashMap<keyType, valueType>::resizeHash()
{
    int filledPercentage = (totalElements * 100) / size;
    if (filledPercentage < 75)
    {
        return;
    }
    else
    {
        int oldSize = size;
        Node<keyType, valueType> **oldBucket = HashTable;
        HashTable = new Node<keyType, valueType> *[size * 2]();
        size = size * 2;
        for (int i = 0; i < oldSize; i++)
        {
            Node<keyType, valueType> *eachHead = oldBucket[i], *deleteNode = nullptr;
            while (eachHead)
            {
                hashInsertion(eachHead->getKey(), eachHead->getValue());
                deleteNode = eachHead;
                eachHead = eachHead->getNext();
                deleteNode->setNext(nullptr);
                delete deleteNode;
            }
        }
        delete[] oldBucket;
    }
}

// | Code  | Type                      |
// | ----- | ------------------------- |
// | `i`   | `int`                     |
// | `d`   | `double`                  |
// | `c`   | `char`                    |
// | `f`   | `float`                   |
// | `PKc` | `const char*`             |
// | `Ss`  | `std::string` (sometimes) |

template <typename keyType, typename valueType>
int HashMap<keyType, valueType>::hashFunction(keyType key) // hashfunction for finding the index in the hashtable bucket
{
    // if (strcmp(typeid(key).name(), "c") == 0 || strcmp(typeid(key).name(), "PKc") == 0 || strcmp(typeid(key).name(), "Ss") == 0)
    // {
    //     int count = 0;
    //     for (auto ch : key)
    //     {
    //         count += int(ch);
    //     }
    //     return count % size;
    // }
    // return key % size;
    return computeHash(key, size);
}

int computeHash(int key, int size)
{
    return key % size;
}
int computeHash(long key, int size)
{
    return key % size;
}
int computeHash(float key, int size)
{
    return (int)key % size;
}
int computeHash(const string &key, int size)
{
    int count = 0;
    for (auto ch : key)
    {
        count += (int)ch;
    }
    return count % size;
}
int computeHash(const char *key, int size)
{
    int count = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        count += (int)key[i];
    }
    return count % size;
}

bool isMatched(int stored, int key)
{
    return stored == key;
}
bool isMatched(long stored, long key)
{
    return stored == key;
}
bool isMatched(float stored, float key)
{
    return stored == key;
}
bool isMatched(const string &stored, const string &key)
{
    if (stored.size() != key.size())
    {
        return false;
    }
    for (int i = 0; i < stored.size(); i++)
    {
        if (stored[i] != key[i])
        {
            return false;
        }
    }
    return true;
}
bool isMatched(const char *stored, const char *key)
{
    if (!stored || !key)
        return false;
    int i = 0;
    while (stored[i] != '\0' && key[i] != '\0')
    {
        if (stored[i] != key[i])
            return false;
        i++;
    }
    return stored[i] == '\0' && key[i] == '\0';
}

template <typename keyType, typename valueType>
Node<keyType, valueType> *HashMap<keyType, valueType>::getNode(keyType key)
{
    int headPosition = hashFunction(key);
    Node<keyType, valueType> *headNode = HashTable[headPosition];
    if (!headNode)
    {
        return nullptr;
    }
    while (headNode)
    {
        if (isMatched(headNode->getKey(), key))
        {
            return headNode;
        }
        headNode = headNode->getNext();
    }
    return nullptr;
}

template <typename keyType, typename valueType>
HashMap<keyType, valueType> HashMap<keyType, valueType>::operator=(HashMap<keyType, valueType> *obj)
{
}

// memory leak in operator overloading