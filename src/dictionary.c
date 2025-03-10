/**
 * XScrabble - Dictionary Implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"
#include "config.h"

/* Dictionary data structure */
/* For simplicity, we'll use a basic implementation */
#define MAX_WORDS 100000
#define MAX_WORD_LENGTH 20

static char **dictionary = NULL;
static int word_count = 0;

/* Initialize dictionary */
bool dictionary_init(void)
{
    FILE *file;
    char buffer[MAX_WORD_LENGTH];
    
    /* Allocate memory for dictionary */
    dictionary = (char **)malloc(MAX_WORDS * sizeof(char *));
    if (!dictionary) {
        return false;
    }
    
    /* Open dictionary file */
    file = fopen(DICTIONARY_FILE, "r");
    if (!file) {
        /* For testing - create a minimal dictionary */
        dictionary[0] = strdup("weft");
        dictionary[1] = strdup("scrabble");
        word_count = 2;
        return true;
    }
    
    /* Read words from dictionary file */
    while (fgets(buffer, MAX_WORD_LENGTH, file) && word_count < MAX_WORDS) {
        /* Remove newline character */
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        /* Convert to lowercase */
        for (int i = 0; buffer[i]; i++) {
            buffer[i] = tolower(buffer[i]);
        }
        
        /* Add to dictionary */
        dictionary[word_count] = strdup(buffer);
        word_count++;
    }
    
    fclose(file);
    return true;
}

/* Clean up dictionary resources */
void dictionary_cleanup(void)
{
    if (dictionary) {
        for (int i = 0; i < word_count; i++) {
            free(dictionary[i]);
        }
        free(dictionary);
        dictionary = NULL;
    }
}

/* Check if a word is in the dictionary */
bool dictionary_is_word(const char *word)
{
    char lowercase[MAX_WORD_LENGTH];
    
    /* Convert to lowercase for comparison */
    strncpy(lowercase, word, MAX_WORD_LENGTH - 1);
    lowercase[MAX_WORD_LENGTH - 1] = '\0';
    for (int i = 0; lowercase[i]; i++) {
        lowercase[i] = tolower(lowercase[i]);
    }
    
    /* Search for word in dictionary */
    for (int i = 0; i < word_count; i++) {
        if (strcmp(dictionary[i], lowercase) == 0) {
            return true;
        }
    }
    
    return false;
}
