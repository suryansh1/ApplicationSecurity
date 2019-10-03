#include <check.h>
#include "dictionary.h"
#include <stdlib.h>

#define DICTIONARY "wordlist.txt"
#define TESTDICT "test_wordlist.txt"

START_TEST(test_dictionary_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(load_dictionary(TESTDICT, hashtable));
    const char* word1 = "justice";
    int bucket1 = 759;

    const char* word2 = "pneumonoultramicroscopicsilicovolcanoconiosis";
    int bucket2 = 880;
    ck_assert(hash_function(word1) == bucket1);
    ck_assert(hash_function(word2) == bucket2);

}
END_TEST

START_TEST(test_check_word_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* correct_word = "justice";
    const char* punctuation_word_1 = "\"place\"";
    const char* punctuation_word_2 = "pl.ace";

    ck_assert(check_word(correct_word, hashtable));
    ck_assert(!check_word(punctuation_word_1, hashtable));
    ck_assert(!check_word(punctuation_word_2, hashtable));

    
}
END_TEST



START_TEST(test_check_words_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char* expected[3];
    expected[0] = "sogn";
    expected[1] = "skyn";
    expected[2] = "betta";
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("test1.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 3);
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
}
END_TEST

START_TEST(test_check_word_long)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    //Checking oversized string is handled by check_word
    
    const char* long_word = 
            "aaaaaaaaaapneumonoultramicroscopicsilicovolcanoconiosisssssssss";

    ck_assert(!check_word(long_word, hashtable));
   

}
END_TEST

START_TEST(test_check_word_mixed_case)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    
    const char* mixed_case_word_1 =  "HeLLo";
    const char* mixed_case_word_2 =  "Hello";
    const char* small_case_word =  "hello";


    /* 
    I am converting each word in dictionary to lower case before storing
    
    Also, each word which is to be checked is being converted to lowercase 
    before being passed to check_word()

    This is done to make sure hashtable does not have multiple entries
    for different case words, and so that we do not have to traverse hash table
    twice, with different case letters to search for a word

    Thus, only small case would be present in dictionary and other cases words
    should not be there
    */
    ck_assert(!check_word(mixed_case_word_1, hashtable));
    ck_assert(!check_word(mixed_case_word_2, hashtable));
    ck_assert(check_word(small_case_word, hashtable));
   

}
END_TEST

Suite *
check_word_suite(void)
{
    Suite * suite;
    TCase * check_word_case;
    suite = suite_create("check_word");
    check_word_case = tcase_create("Core");
    tcase_add_test(check_word_case, test_check_word_normal);
    tcase_add_test(check_word_case, test_check_words_normal);
    tcase_add_test(check_word_case, test_dictionary_normal);
    tcase_add_test(check_word_case, test_check_word_long);
    tcase_add_test(check_word_case, test_check_word_mixed_case);
    suite_add_tcase(suite, check_word_case);

    return suite;
}

int
main(void)
{
    int failed;
    Suite *suite;
    SRunner *runner;
    
    suite = check_word_suite();
    runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

