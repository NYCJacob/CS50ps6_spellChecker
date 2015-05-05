/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * By Jacob Sherman EdX student 2014
 * jacob@jbsherman.com
 *
 * Implements a dictionary's functionality.  This one uses hash table.
 *
 ***************************************************************************/

#include <stdbool.h>
#include "dictionary.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*table size N should be a prime number to make compression function efficient*/
/*otherwise module 4 would result in hash number divisible by 4 and therefore */
/*not use 3/4 of the hash table- UCBerkley 61B*/

// #define buckets 393241  this time was .
#define buckets 196613
#define loadLimit 0.8 // need to pick threshold load constant

/*if load factor stays low abd hash code and compresion function are "good*/
/*and no duplicate keys, then chains are short and each operation takes*/
/*O(1) constant time.-  UC Berkley CS61B*/
// large prime number for hash table size (may not have to be prime based on compression function

typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
}
node;

node* hashTable[buckets] = {NULL};   // hashtable array init and null   

int entries = 0;   // made global to share between load and size functions - better way?

unsigned int DJBHash(char* str)
{
    unsigned int hash = 5381;
    unsigned int i;
    int wordLen = strlen(str);

    for(i = 0; i < wordLen; str++, i++)
    {
        hash = ((hash << 5) + hash) + (*str);
    }
    hash = hash % buckets;
    return hash;
}


/*unsigned int UCBHash(char* key)   // adapted from UC Berkley CS61B java lecture*/
/*{*/
/*    unsigned int hashCode = 0;*/
/*    for (int i = 0; i < strlen(key); i++)*/
/*    {*/
/*        hashCode = (127 * hashCode + key[i]) % 16908799;     // large prime number large than buckets for 32bit int*/
/*    }*/
/*    hashCode = hashCode % buckets;                  // should use a compression function*/
/* // compression function from UC Berkley CS61B Lecture 21-- now number of buckets does not need to be prime*/
/*    */
/*    return hashCode;*/
/*}*/

/*unsigned int resizeTable(entries)   // if load factor too big you loose constant time O(1)*/
/*{*/
/*    // double size of array*/
/*    // walk through old array and rehash into new array*/
/*}*/

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    int wordLen =  strlen(word);
    char word2check[wordLen + 1];
    node* checker;
    for (int x = 0; x < wordLen; x++)
    {
        if (isalpha(word[x]) && isupper(word[x]))
        {
            word2check[x] = tolower(word[x]);
        }
        else 
            word2check[x] = word[x];
    }
    word2check[wordLen] = '\0';
   // printf("word2check = %s\n", word2check);
    unsigned int chkHash = DJBHash(word2check);
    checker = hashTable[chkHash];

    while (checker != NULL)
    {
        if (strcmp(checker->word, word2check) == 0)
            return true;
        else
            checker = checker->next;
    }
    return false;
}


/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
   // open dictionary file    
    FILE* fpd = fopen(dictionary, "r");
    if (fpd == NULL)
    {
        printf("Could not open dictionary.\n");
        unload();
        return 11;
    }

    char currWord[LENGTH +1];   
    while(fscanf(fpd, "%s", currWord) == 1)
    {   
        node* new_node = malloc(sizeof(node));
        strcpy(new_node->word, currWord);
        
        unsigned int hashIndex = DJBHash(new_node->word);
       // printf("hash value is %d\n", hashIndex);
        if (hashTable[hashIndex] == NULL)
        {
           // printf("hashTable bucket is null\n");
            new_node->next = NULL;  
            hashTable[hashIndex] = new_node;
        }
        else if  (hashTable[hashIndex] != NULL)
        {
          //  printf("hash value already used- will chain.\n");
            new_node->next = hashTable[hashIndex];
            hashTable[hashIndex] = new_node;
        }
        else
            printf("something wrong in hash lookup \n");
        entries++;
      }
    
        if (ferror(fpd))
        {
            printf("There was an error reading the dictionary.\n");    
        }
        fclose(fpd);  
        return true;
}  /// end of bool load

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return entries;
}


/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    for (int freeX = 0; freeX < buckets; freeX++)
    {
        if (hashTable[freeX] != NULL)
        {
            node* cursor = hashTable[freeX];
            while (cursor != NULL)
            {
                node* temp = cursor;
                cursor = cursor->next;
                free(temp);
            }
        }
    }
    return true;
}
