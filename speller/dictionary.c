// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Another useful global variables
static unsigned int hashValue;
static unsigned int words = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // 1. Invoke hash function & set current cursor position
    hashValue = hash(word);
    node *cursor = table[hashValue];

    // 2. Iterate over list lookin' for the word
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
            return true;
        else
            cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
// Function based on @RealisticResponse YouTube channel
unsigned int hash(const char *word)
{
    unsigned long total = 0;
    for (int i = 0; i < strlen(word); i++)
        total += N * tolower(word[i]);
    return (total % N);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *f = fopen(dictionary, "r");
    if (f == NULL)
        return false;

    // Let's read the file: I'll use an static char array
    char line[LENGTH + 1];
    while (fscanf(f, "%s", line) != EOF)
    {
        // 1. Allocate mem for the node
        node *n = malloc(sizeof(node));
        if (n == NULL)
            return false;

        // 2. Copy the line into content node
        strcpy(n->word, line);

        // 3. Invoke the Hash function
        hashValue = hash(line);

        // 4. Have I reached the end of hashmap?
        if (table[hashValue] == NULL)
            n->next = NULL;
        else
            n->next = table[hashValue];
        table[hashValue] = n;
        words++;
    }
    fclose(f);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (words > 0)
        return words;
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor)
        {
            node *aux = cursor;
            cursor = cursor->next;
            free(aux);
        }
    }
    return true;
}
