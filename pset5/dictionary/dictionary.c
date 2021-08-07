// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 500000;

// Hash table
node *table[N];

// Number of words loaded into dictionary
int dict_size = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Calls hash function to select linked list within array
    int hash_value = hash(word);

    // Go to the selected linked list
    node *cursor = table[hash_value];

    // Traverse the linked list until cursor reaches NULL
    while (cursor)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int number = 5381;
    int c = 0;

    while (c == *word++)
    {
        number = ((number << 5) + number) + c;
        // * number * 33 + c */
    }

    return number % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Opens dictionary file
    FILE *dict_file = fopen(dictionary, "r");
    if (!dict_file)
    {
        return false;
    }

    // Recursively scans each word to be loaded into dictionary until EOF
    char scanned_word[LENGTH + 1];
    while (fscanf(dict_file, "%s", scanned_word) != EOF)
    {
        // Allocates memory for the most recently scanned word
        node *new_node = malloc(sizeof(node));
        if (!new_node)
        {
            return false;
        }

        // Copies the most recently scanned word into new node
        strcpy(new_node->word, scanned_word);
        new_node->next = NULL;

        // Calls hash function to select linked list within the array
        int hash_value = hash(scanned_word);

        // Inserts new node into selected linked list
        if (!table[hash_value])
        {
            table[hash_value] = new_node;
        }
        else
        {
            new_node->next = table[hash_value];
            table[hash_value] = new_node;
        }
        dict_size++;
    }

    fclose(dict_file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dict_size;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Goes to each linked list in the array
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        node *tmp = table[i];

        // Traverse the linked list
        while (cursor)
        {
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }
    }
    return true;
}
