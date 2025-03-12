/**
 * XScrabble - Enhanced Dictionary Implementation with Definitions Support
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "dictionary_enhanced.h"
#include "config.h"

/* For JSON parsing - this is a simplified mock implementation */
/* In a real implementation, you would use a JSON library like cJSON */
typedef struct {
    char *data;
    size_t size;
} JSONBuffer;

/* Dictionary data structure */
#define MAX_ENTRIES 100000

static DictionaryEntry *dictionary = NULL;
static int entry_count = 0;

/* Simple JSON parsing functions - these are mocks for demonstration */
static char* json_extract_string(const char *json, const char *key);
static JSONBuffer read_file(const char *filename);

/* Initialize dictionary */
bool dictionary_init(void)
{
    /* Allocate memory for dictionary */
    dictionary = (DictionaryEntry *)malloc(MAX_ENTRIES * sizeof(DictionaryEntry));
    if (!dictionary) {
        return false;
    }
    
    /* Add some default entries for testing */
    strcpy(dictionary[0].word, "weft");
    dictionary[0].definition = strdup("A thread that crosses horizontally through the warp threads in weaving.");
    dictionary[0].example = strdup("The weft is passed over and under the warp threads.");
    dictionary[0].part_of_speech = strdup("noun");
    
    strcpy(dictionary[1].word, "scrabble");
    dictionary[1].definition = strdup("A board game in which players use letters to form words on a grid.");
    dictionary[1].example = strdup("We play Scrabble every weekend.");
    dictionary[1].part_of_speech = strdup("noun");
    
    entry_count = 2;
    
    /* Try to load French dictionary if available */
    dictionary_load_json("data/dictionaries/extracted/french_dict_sample.json");
    
    return true;
}

/* Clean up dictionary resources */
void dictionary_cleanup(void)
{
    if (dictionary) {
        for (int i = 0; i < entry_count; i++) {
            free(dictionary[i].definition);
            if (dictionary[i].example) free(dictionary[i].example);
            if (dictionary[i].part_of_speech) free(dictionary[i].part_of_speech);
        }
        free(dictionary);
        dictionary = NULL;
    }
}

/* Check if a word is in the dictionary */
bool dictionary_is_word(const char *word)
{
    char lowercase[32];
    
    /* Convert to lowercase for comparison */
    strncpy(lowercase, word, sizeof(lowercase) - 1);
    lowercase[sizeof(lowercase) - 1] = '\0';
    for (int i = 0; lowercase[i]; i++) {
        lowercase[i] = tolower(lowercase[i]);
    }
    
    /* Search for word in dictionary */
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(dictionary[i].word, lowercase) == 0) {
            return true;
        }
    }
    
    return false;
}

/* Look up a word in the dictionary and return its entry */
const DictionaryEntry* dictionary_lookup(const char *word)
{
    char lowercase[32];
    
    /* Convert to lowercase for comparison */
    strncpy(lowercase, word, sizeof(lowercase) - 1);
    lowercase[sizeof(lowercase) - 1] = '\0';
    for (int i = 0; lowercase[i]; i++) {
        lowercase[i] = tolower(lowercase[i]);
    }
    
    /* Search for word in dictionary */
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(dictionary[i].word, lowercase) == 0) {
            return &dictionary[i];
        }
    }
    
    return NULL;
}

/* Get the definition of a word */
const char* dictionary_get_definition(const char *word)
{
    const DictionaryEntry *entry = dictionary_lookup(word);
    return entry ? entry->definition : NULL;
}

/* Get the example usage of a word */
const char* dictionary_get_example(const char *word)
{
    const DictionaryEntry *entry = dictionary_lookup(word);
    return entry ? entry->example : NULL;
}

/* Get the part of speech of a word */
const char* dictionary_get_part_of_speech(const char *word)
{
    const DictionaryEntry *entry = dictionary_lookup(word);
    return entry ? entry->part_of_speech : NULL;
}

/* Load dictionary from JSON file */
bool dictionary_load_json(const char *filename)
{
    JSONBuffer buffer = read_file(filename);
    if (!buffer.data) {
        printf("Failed to load dictionary file: %s\n", filename);
        return false;
    }
    
    /* This is a very simplified mock JSON parser */
    /* In a real implementation, use a JSON library */
    char *ptr = buffer.data;
    char *end = buffer.data + buffer.size;
    
    /* Skip opening brace */
    if (*ptr != '{') {
        free(buffer.data);
        return false;
    }
    ptr++;
    
    /* Parse each word entry */
    int loaded = 0;
    while (ptr < end && entry_count < MAX_ENTRIES) {
        /* Find the next word key */
        char *quote = strchr(ptr, '"');
        if (!quote) break;
        
        char *word_start = quote + 1;
        char *word_end = strchr(word_start, '"');
        if (!word_end) break;
        
        /* Extract the word */
        size_t word_len = word_end - word_start;
        if (word_len >= sizeof(dictionary[entry_count].word)) {
            word_len = sizeof(dictionary[entry_count].word) - 1;
        }
        strncpy(dictionary[entry_count].word, word_start, word_len);
        dictionary[entry_count].word[word_len] = '\0';
        
        /* Find the definition section */
        ptr = strstr(word_end, "\"definition\"");
        if (!ptr) break;
        
        /* Extract definition */
        char *def_value = strstr(ptr, ":");
        if (!def_value) break;
        def_value = strchr(def_value, '"');
        if (!def_value) break;
        
        char *def_start = def_value + 1;
        char *def_end = strchr(def_start, '"');
        if (!def_end) break;
        
        size_t def_len = def_end - def_start;
        dictionary[entry_count].definition = (char *)malloc(def_len + 1);
        strncpy(dictionary[entry_count].definition, def_start, def_len);
        dictionary[entry_count].definition[def_len] = '\0';
        
        /* Extract example (optional) */
        ptr = strstr(def_end, "\"example\"");
        if (ptr) {
            char *example_value = strstr(ptr, ":");
            if (example_value) {
                example_value = strchr(example_value, '"');
                if (example_value) {
                    char *example_start = example_value + 1;
                    char *example_end = strchr(example_start, '"');
                    if (example_end) {
                        size_t example_len = example_end - example_start;
                        dictionary[entry_count].example = (char *)malloc(example_len + 1);
                        strncpy(dictionary[entry_count].example, example_start, example_len);
                        dictionary[entry_count].example[example_len] = '\0';
                    }
                }
            }
        } else {
            dictionary[entry_count].example = NULL;
        }
        
        /* Extract part of speech (optional) */
        ptr = strstr(def_end, "\"part_of_speech\"");
        if (ptr) {
            char *pos_value = strstr(ptr, ":");
            if (pos_value) {
                pos_value = strchr(pos_value, '"');
                if (pos_value) {
                    char *pos_start = pos_value + 1;
                    char *pos_end = strchr(pos_start, '"');
                    if (pos_end) {
                        size_t pos_len = pos_end - pos_start;
                        dictionary[entry_count].part_of_speech = (char *)malloc(pos_len + 1);
                        strncpy(dictionary[entry_count].part_of_speech, pos_start, pos_len);
                        dictionary[entry_count].part_of_speech[pos_len] = '\0';
                    }
                }
            }
        } else {
            dictionary[entry_count].part_of_speech = NULL;
        }
        
        /* Move to next entry */
        entry_count++;
        loaded++;
        
        /* Find next word or end of data */
        ptr = strchr(def_end, '}');
        if (!ptr) break;
        ptr++;
    }
    
    free(buffer.data);
    printf("Loaded %d words from %s\n", loaded, filename);
    return loaded > 0;
}

/* Read a file into a buffer */
static JSONBuffer read_file(const char *filename)
{
    JSONBuffer buffer = {NULL, 0};
    FILE *file = fopen(filename, "r");
    if (!file) {
        return buffer;
    }
    
    /* Get file size */
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    /* Allocate buffer */
    buffer.data = (char *)malloc(size + 1);
    if (!buffer.data) {
        fclose(file);
        return buffer;
    }
    
    /* Read file content */
    size_t bytes_read = fread(buffer.data, 1, size, file);
    buffer.data[bytes_read] = '\0';
    buffer.size = bytes_read;
    
    fclose(file);
    return buffer;
}