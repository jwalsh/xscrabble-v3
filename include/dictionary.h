/**
 * XScrabble - Dictionary Definitions
 */

#ifndef XSCRABBLE_DICTIONARY_H
#define XSCRABBLE_DICTIONARY_H

#include <stdbool.h>

/* Function prototypes */
bool dictionary_init(void);
void dictionary_cleanup(void);
bool dictionary_is_word(const char *word);

#endif /* XSCRABBLE_DICTIONARY_H */
