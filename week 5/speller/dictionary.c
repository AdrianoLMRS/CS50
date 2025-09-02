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

// (26 = one for each letter A–Z)
const unsigned int N = 26;

node *table[N];

unsigned int word_count = 0; // Keep track

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int idx = hash(word);

    // Traverse linked list at this bucket
    for (node *cursor = table[idx]; cursor != NULL; cursor = cursor->next)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true; // Word found
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    char c = toupper((unsigned char)word[0]);
    if (c < 'A' || c > 'Z')
    {
        return 0; // not a letter
    }
    return (unsigned int)(c - 'A'); // Map chars to numbers
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    for (unsigned int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    char w[LENGTH + 1];

    // Read words one at a time
    while (fscanf(file, "%45s", w) != EOF)
    {
        // Allocate a new node for each word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fclose(file);
            return false; // fail
        }

        strcpy(n->word, w); // Copy

        unsigned int idx = hash(w);

        // Insert at the beginning of the linked list
        n->next = table[idx];
        table[idx] = n;

        word_count++;
    }
    
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (unsigned int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
        table[i] = NULL;
    }
    return true;
}