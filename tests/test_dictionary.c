/**
 * XScrabble - Dictionary Tests
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/dictionary.h"

int main(void)
{
    printf("Running dictionary tests...\n");
    
    /* Test dictionary initialization */
    assert(dictionary_init());
    
    /* Test word lookup - these should succeed with our test dictionary */
    assert(dictionary_is_word("weft"));
    assert(dictionary_is_word("scrabble"));
    
    /* Test case insensitivity */
    assert(dictionary_is_word("Weft"));
    assert(dictionary_is_word("SCRABBLE"));
    
    /* Test invalid words */
    assert(!dictionary_is_word("xyzzy"));
    assert(!dictionary_is_word("qqq"));
    
    /* Clean up */
    dictionary_cleanup();
    
    printf("Dictionary tests passed!\n");
    return EXIT_SUCCESS;
}
