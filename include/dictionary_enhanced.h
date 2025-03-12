/**
 * XScrabble - Enhanced Dictionary Definitions with Word Definitions Support
 */

#ifndef XSCRABBLE_DICTIONARY_ENHANCED_H
#define XSCRABBLE_DICTIONARY_ENHANCED_H

#include <stdbool.h>

/* Dictionary entry structure with definitions */
typedef struct {
    char word[32];              /* The word itself */
    char *definition;           /* Definition of the word */
    char *example;              /* Example usage (optional) */
    char *part_of_speech;       /* Part of speech (optional) */
} DictionaryEntry;

/* Function prototypes */
bool dictionary_init(void);
void dictionary_cleanup(void);
bool dictionary_is_word(const char *word);

/* Enhanced functions for language learning */
const DictionaryEntry* dictionary_lookup(const char *word);
const char* dictionary_get_definition(const char *word);
const char* dictionary_get_example(const char *word);
const char* dictionary_get_part_of_speech(const char *word);

/* Dictionary management */
bool dictionary_load_json(const char *filename);

#endif /* XSCRABBLE_DICTIONARY_ENHANCED_H */