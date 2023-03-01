// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// Represents a trie
node *root;

//Sözlükteki toplam sayıyı tutan değişken
int n = 0;

int keys(char word)
{
    //Tırnak işareti ise 26 özel değer döndür.
    if(word == 39)
    {
        return 26;
    }
    return tolower(word) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));

    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");

    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    //Kelimenin uzunluğunu tutan değişken.
    int ext;

    //Gezici işaretçi
    node *trv;

    //Anahtar değişkeni.
    int key;

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        //Kelimenin uzunluğu bulunur.
        ext = strlen(word);



        //Gezici işaretçi root'un yerini alır.
        trv = root;

        for(int i = 0,y = ext; i < y; i++)
        {
            //Anahtar bulunur.
            key = keys(word[i]);

            //Yol kapalı, yeni bir yol ve düğüm yarat , trv ve trv child ona işaret etsin.
            if(trv->children[key] == NULL)
            {
                node *nv = malloc(sizeof(node));
                nv->is_word = false;
                for(int j = 0; j < N; j++)
                {
                    nv->children[j] = NULL;
                }

                trv->children[key] = nv;
                trv = nv;
            }

            //Yol açık , trv'yi child'ın baktığı düğüme ilerlet.
            else
            {
                trv = trv->children[key];
            }
        }

        //Kelimenin var olduğu bildirisi.
        trv->is_word = true;
        n++;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return n;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    if(word != NULL)
    {
        //Kelime uzunluğu.
        int ext = strlen(word);

        //Anahtarımız.
        int key;

        //Word dizisi
        char word_[ext+1];

        //Gezici değişken root'dan başlar.
        node* trv = root;

        /*
        for(int j = 0; j < ext; j++)
        {
            word_[j] = tolower(word[j]);
        }
        word_[ext] = '\0';
        */


        for(int i = 0; i < ext; i++)
        {
            //problem
            key = keys(word[i]);

            //Çıkmaz yol , kelime sözlükte bulunmuyor , false döndürür.
            if(trv->children[key] == NULL)
            {
                return false;
            }
            trv = trv->children[key];
        }

        //Tüm yolları geçti , son olarak bu bir kelime ise true döner.
        if(trv->is_word == true)
        {
            return true;
        }
    }

    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //Gezici değişken root'dan başlar.
    node *trv;

    for(int i = 0; i < N; i++)
    {
        trv = root;
        trv = trv->children[i];

        while(trv != NULL)
        {
            node *tmp = trv;
            trv = trv->children[i];
            free(tmp);
        }
    }
    if(root)
    {
        free(root);
        return true;
    }


    return false;
}
