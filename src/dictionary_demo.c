/**
 * XScrabble - French Dictionary Demo
 * 
 * This is a simple program to demonstrate how to use the enhanced
 * dictionary with French words and definitions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary_enhanced.h"

void print_word_info(const char *word) {
    const DictionaryEntry *entry = dictionary_lookup(word);
    
    if (!entry) {
        printf("Word '%s' not found in dictionary.\n", word);
        return;
    }
    
    printf("=== %s ===\n", word);
    printf("Definition: %s\n", entry->definition ? entry->definition : "N/A");
    
    if (entry->part_of_speech) {
        printf("Part of speech: %s\n", entry->part_of_speech);
    }
    
    if (entry->example) {
        printf("Example: %s\n", entry->example);
    }
    
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (!dictionary_init()) {
        fprintf(stderr, "Failed to initialize dictionary.\n");
        return 1;
    }
    
    printf("XScrabble French Dictionary Demo\n");
    printf("================================\n\n");
    
    /* Display sample words */
    print_word_info("bonjour");
    print_word_info("scrabble");
    print_word_info("café");
    print_word_info("pomme");
    
    /* Interactive mode if requested */
    if (argc > 1 && strcmp(argv[1], "-i") == 0) {
        char word[32];
        printf("Enter a word to look up (or 'quit' to exit): ");
        while (scanf("%31s", word) == 1) {
            if (strcmp(word, "quit") == 0) {
                break;
            }
            
            print_word_info(word);
            printf("Enter a word to look up (or 'quit' to exit): ");
        }
    }
    
    dictionary_cleanup();
    return 0;
}