#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"
#undef calculate
#undef getrusage
#define expected_size 118819
// default dictionary
#define DICTIONARY "wordlist.txt"
#define BODICTIONARY "bo_wordlist.txt"
#define MAX_MISSPELLED 1000

#define MAX_MISSPELLED 1000


int main(int argc, char** argv) 

{
    hashmap_t hashtable[HASH_SIZE];
    if (argc < 3) 

    {
        fprintf(stderr, "Error: Insufficient arguments!\n");
        fprintf(stderr, "Usage: ./program to_check.txt wordlist.txt\n");
        exit(-1);
    }
    char read_mode[2] = "r";
    
    FILE* fp = fopen(argv[1], read_mode);
    
    char * dictionary = argv[2]; 

    printf("%s is the dictionary file\n",dictionary);
        
    bool load_success = load_dictionary(dictionary, hashtable);

    if (!load_success)
    {
        printf("\n\nCould not read dictionary file, exitting\n");
        return -1;

    }

    char* misspelled[MAX_MISSPELLED];
    
    int num_wrong = check_words(fp, hashtable, misspelled);
    
    int i = 0;
    for (i = 0; i < num_wrong; i++) 
        {
            printf("\n%s\n", misspelled[i]);
            free(misspelled[i]);
        }

    fclose( fp );

    //Delete hashtable and misspelled
    for (i=0; i<HASH_SIZE; i++)
        if(hashtable[i]!=NULL)
            
            free(hashtable[i]);

    return 0;
}
