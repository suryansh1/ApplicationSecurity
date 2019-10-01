#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

#define MAX_LINE_LENGTH 909600

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[])
{

    // Set int num_misspelled to 0.

    int num_misspelled = 0, len = 0;

    char word[LENGTH], ch, *read_word, line[MAX_LINE_LENGTH];

    // while (1)
    // {
        
    //     if (feof(fp))
    //         break;

    //     ch = fgetc(fp);

    //     if (isalpha(ch) || ch == '\'')
    //         word[len++] = tolower(ch);

    //     else if(len >0)
    //     {
    //         word[len]  = '\0';
    //         if (!check_word(word, hashtable))
    //         {
    //             // printf("\n%s is misspelled, adding to misspelled ", word);

    //             misspelled[num_misspelled] = malloc((len+1) * sizeof(char));
    //             strcpy(misspelled[num_misspelled++], word);
    //         }

    //         len = 0;
    //         strcpy(word,"\0");
    //     }
    // }

    // While line in fp is not EOF (end of file):
    //     Read the line
    char delimit[]=" \t\r\n\v\f";

    while (fgets(line, MAX_LINE_LENGTH, fp)) 
    {
        size_t line_len = strlen(line);
        if (line_len && (line[line_len - 1] != '\n')) 
        {
            // printf("\nLine exceeds designated length, skipping ");
            continue;
        }

        // Removing trailing newline
        line[line_len - 1] = '\0';
        
        // printf("\n Read Line : %s\n", line);
    //     Split the line on spaces.
        read_word = strtok(line, delimit);

    //     For each word in line:
        while (read_word != NULL)
        {
    
//         Remove punctuation from beginning and end of word.
            size_t read_word_len = strlen(read_word);

            int start_index = 0, end_index = read_word_len - 1;

            while(start_index < read_word_len)
            {
                if (ispunct(read_word[start_index]))
                    start_index++;
                else
                    break;
            }

            while(end_index > 0)
            {
                if (ispunct(read_word[end_index]))
                    end_index--;
                else
                    break;
            }
            int ctr = 0, rctr = 0;

            for (ctr = 0, rctr = start_index;
                    ctr<= end_index - start_index; ctr++,rctr++)
    
                    word[ctr] = tolower(read_word[rctr]);

            word[ctr] = '\0';
            
            if(ctr <= LENGTH)
            {        
        //      If not check_word(word):
                if (!check_word(word, hashtable))
                {
                    // printf("\n%s is misspelled, adding to misspelled ", word);

        //             Append word to misspelled.
                    misspelled[num_misspelled] = malloc((ctr+1) * sizeof(char));
        //             Increment num_misspelled.
                    strcpy(misspelled[num_misspelled++], word);
                }
            }

            read_word = strtok(NULL, delimit);

        }
    
    }
    return num_misspelled;
}

/**
 * Returns true if word is in dictionary else false.
 */
/**
 * Inputs:
 *  word:       A word to check the spelling of.
 *  hashtable:  The hash table used to determine spelling
 *            
 * Returns:
 *  bool:       A boolean value indicating if the word was correctly spelled.
 *
 * Modifies:
 *  
 * Example:
 *  bool correct  = check_word(word, hashtable);
 **/
bool check_word(const char* word, hashmap_t hashtable[])
{
        
    // Set int bucket to the output of hash_function(word).
    int bucket = hash_function(word);

    // printf("\n\nBucket for %s is %d\n", word, bucket );

    // printf("Value at hashtable for bucket 552 : %s", hashtable[552]->word);

    if(!hashtable[bucket])
    {   
        // printf("\n\nNo entry in hashtable, %s is misspelled, \n", word);
        
        return false;
    }

    // Set hashmap_t cursor equal to hashtable[bucket].
    // printf("Value at hashtable for bucket : %s", hashtable[bucket]->word);
    hashmap_t cursor = hashtable[bucket];

    // While cursor is not NULL:
    while(cursor != NULL)
    {
        //     If word equals cursor->word:
        //         return True.
        // printf("\nIterating hashtable for bucket %d : %s\n", bucket, 
        //              cursor -> word);

        if(!strcmp(word, cursor->word))
            return true;

        //     Set cursor to cursor->next.
        cursor = cursor -> next;
                
    }

    return false;
    
    // Will be providing lowercase input to function so skipping 


    // Set int bucket to the output of hash_function(word).
    // Set hashmap_t cursor equal to hashmap[bucket].
    // While cursor is  not NULL:
    //     If lower_case(word) equals curosr->word:
    //         return True.
    //     Set curosr to cursor->next.
    // return False.
    
}

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[])
{
    // Initialize all values in hash table to NULL.

    int i = 0;
    for ( i = 0; i< HASH_SIZE; ++i)
        hashtable[i] = NULL;

    // printf("\n\nInitialized hashtable of size %d to NULL\n", i);

    // Checking null values
    // printf("%ld\n", hashtable[552]);

    // Open dict_file from path stored in dictionary.
    char read_mode[2] = "r";

    FILE* fp = fopen(dictionary_file, read_mode);

    // If dict_file is NULL:
    //     return false.

    if (fp == NULL)
    {
        // printf("Dictionary File %s is empty\n\n", dictionary_file);
        return false;
    }

    char word[LENGTH];

    // While word in dict_file is not EOF (end of file):

    while (fgets(word, LENGTH, fp)) 
    {
        size_t len = strlen(word);
        if (len && (word[len - 1] != '\n')) 
        {
            // printf("Word exceeds designated length, skipping \n");
            continue;
        }

        // Removing trailing newline
        word[len - 1] = '\0';

        // Converting Word to lowercase as spelling is case insensitive
        for (int i = 0; i < len - 1; ++i)
            word[i] = tolower(word[i]);

    //     Set hashmap_t new_node to a new node.

        node * new_node = (node *) malloc(sizeof(node));
        // printf("%s\n", new_node-> word );

        if (new_node == NULL)
        {            
            // printf("Could not initialize hash table \n");
            return false;
        }
    //     Set new_node->next to NULL.
        new_node -> next = NULL;

    //     Set new_node->word equal to word.
        strcpy(new_node -> word, word);
        // printf("%s\n", new_node->word);

    //     Set int bucket to hash_function(word).
        int bucket = hash_function(word);

        // printf("\nhash_function value for %s is %d", word, bucket);

        if(hashtable[bucket] == NULL)
    
        //  Set hashtable[bucket] to new_node.
            hashtable[bucket] = new_node;
    
        else
        {
            // Set new_node->next to hashtable[bucket].
            new_node -> next = hashtable[bucket];

            // Set hashtable[bucket] to new_node.
            hashtable [bucket] = new_node;

        }

        // printf("\nAdded %s to the hashtable \n", hashtable[bucket]->word );

    }
    // Close dict_file.
    fclose( fp );
    return true;
}

