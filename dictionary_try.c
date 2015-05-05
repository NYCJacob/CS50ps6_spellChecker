/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdbool.h>

#include "dictionary.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//  speller.c already has #include <stdio.h>


    
// trie structure

    typedef struct node
    {
        bool is_word;
        struct node* children[27];
    }
    node;
    node* root;    // root of trie tree

    int dictWordCount = 0;   // made global to share between load and size functions - better way?

/************   free linked list memory  **/
bool freeNodes(node* freeConductor);

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
   // printf("starting check function\n");
    int wordInt;  // int value of letter from word array passed to function
    struct node* checkRoot = root;
    int checking = 0;   //  counter through char array of current word being checked in while loop
    
    while (word[checking] != '\0')
    {
        if (word[checking] == '\'')
            wordInt = 0;
            else
                wordInt = tolower(word[checking]) - 96;        
   
        if (checkRoot->children[wordInt] != NULL)
        {
            checkRoot = checkRoot->children[wordInt];
            checking++;
        }
        else 
            return false;

    }  // end while loop
    if (checkRoot->is_word == true)
    {
        return true;
    }
    else
    {
       // printf("word not in dictionary\n");
        return false;
    }
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
   root = malloc(sizeof(struct node));  // assign root pointer to a node
    for(int x = 0; x < 27; x++)
    {
        root->children[x] = NULL;
    }
   
// open dictionary file    
   FILE* fpd = fopen(dictionary, "r");
   if (fpd == NULL)
   {
        printf("Could not open dictionary.\n");
        unload();
        return 11;
    }

// char array of defined LENGTH of max length in dictionary
    char dictWord[LENGTH +1];
    int letterInt; 
    struct node* conductor = NULL;  // will point to each node as it moves through the trie
    int cursor;      //  position in word ie the lettr being checked
    int i;  // counter to initialize children to null
     
        while(fscanf(fpd, "%s", dictWord) == 1)
        {   
            conductor = root;       // set to root at start 
            int wordLength = strlen(dictWord);  //  fscanf does not include /n
            
            for (cursor = 0; cursor < wordLength; cursor++)
            {
                if (dictWord[cursor] == '\'')
                    letterInt = 0;
                    else
                        letterInt = tolower(dictWord[cursor]) - 96;
                if (conductor->children[letterInt] == NULL)
                {
                        conductor->children[letterInt] = malloc(sizeof(node));
                        conductor->children[letterInt]-> is_word = false;
                        for (i=0; i<27; i++)
                        {
                            conductor->children[letterInt]->children[i] = NULL;
                        }
                       // printf("new node- level %d - letterInt %d\n", cursor, letterInt);
                 }                  
                 conductor = conductor->children[letterInt];
              }
              conductor-> is_word = true;
              dictWordCount++;
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
    return dictWordCount;
}

/****  freeNodes frees trie node memory   ***/
//** recursion not null loop takes you to bottom of trie where children null free current node
bool freeNodes(node* freeRoot)
{
    for (int iFree = 0; iFree < 27; iFree++)
    {
        if (freeRoot->children[iFree] != NULL)
        {
            freeNodes(freeRoot->children[iFree]);  // recursive till at last or bottom
        }

    }
    free(freeRoot);  //  since recursive call done from bottom this is freeRoot position
    return true;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
   
    freeNodes(root);
    return true;
}
