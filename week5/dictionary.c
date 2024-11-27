// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// number of words in loaded dictionary
unsigned int num = 0;

// compare two strings ignoring case
int my_strcasecmp(const char *str1, const char *str2)
{
    while (*str1 && *str2)
    {
        if (tolower((unsigned char) *str1) != tolower((unsigned char) *str2))
            return 1;
        str1++;
        str2++;
    }

    return (*str1 == '\0' && *str2 == '\0') ? 0 : 1;
}
// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int hash1 = hash(word);
    node *node_word = table[hash1];
    while (node_word != NULL)
    {
        if (my_strcasecmp(word, node_word->word) == 0)
        {
            return true;
        }
        else
            node_word = node_word->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *dic = fopen(dictionary, "r");
    if (dic == NULL)
    {
        return false;
    }

    int len_char = sizeof(char);
    int i = 0;
    char letter;
    char word[LENGTH + 1];
    while (fread(&letter, len_char, 1, dic))
    {
        if (isalpha(letter) || (letter == '\'' && i > 0))
        {
            word[i] = letter;
            i++;
        }

        else if (i > 0 && letter == '\n')
        {
            word[i] = '\0';

            node *new_node = malloc(sizeof(node));
            if (new_node == NULL)
            {
                fclose(dic);
                return false;
            }

            strcpy(new_node->word, word);
            new_node->next = NULL;

            int hash1 = hash(word);
            if (table[hash1] == NULL)
                table[hash1] = new_node;
            else
            {
                new_node->next = table[hash1];
                table[hash1] = new_node;
            }
            num++;
            i = 0;
        }
    }

    fclose(dic);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return num;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;    // 保存当前节点
            cursor = cursor->next; // 移动到下一个节点
            free(tmp);             // 释放当前节点的内存
        }
        if (i == N - 1 && cursor == NULL)
            return true;
    }

    return false;
}
