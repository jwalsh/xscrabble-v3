/**
 * XScrabble - AL Dictionary Demo
 * 
 * This demonstrates how to use the made-up "AL" dictionary words
 * for language learning in a dual-lingual Scrabble game.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* Dictionary entry structure with definitions */
typedef struct {
    char word[32];
    char *definition;
    char *english_definition;
    char *example;
    char *part_of_speech;
} ALDictionaryEntry;

#define MAX_ENTRIES 1000
#define MAX_LINE_LENGTH 256

static ALDictionaryEntry *dictionary = NULL;
static int entry_count = 0;
static char **word_list = NULL;
static int word_count = 0;

/* Function prototypes */
bool load_word_list(const char *filename);
bool load_definitions(const char *filename);
void cleanup(void);
void print_word_info(const char *word);
const ALDictionaryEntry* lookup_word(const char *word);
void list_all_words(void);
void random_quiz(int num_questions);
void search_by_pattern(const char *pattern);

int main(int argc, char *argv[]) {
    printf("XScrabble - AL Dictionary Learning Demo\n");
    printf("=======================================\n\n");

    /* Initialize dictionary with word list and definitions */
    dictionary = (ALDictionaryEntry *)malloc(MAX_ENTRIES * sizeof(ALDictionaryEntry));
    if (!dictionary) {
        fprintf(stderr, "Failed to allocate memory for dictionary\n");
        return 1;
    }

    /* Load word lists and definitions */
    if (!load_word_list("/Users/jasonwalsh/projects/xscrabble-v3/data/dictionaries/extracted/AL_TEMPLATE.txt")) {
        fprintf(stderr, "Failed to load main word list\n");
        cleanup();
        return 1;
    }
    
    /* Load short words list as well */
    if (!load_word_list("/Users/jasonwalsh/projects/xscrabble-v3/data/dictionaries/extracted/AL_SHORT_WORDS.txt")) {
        fprintf(stderr, "Warning: Failed to load short words list\n");
        /* Continue anyway with just the main word list */
    }

    /* Load standard definitions */
    if (!load_definitions("/Users/jasonwalsh/projects/xscrabble-v3/data/dictionaries/extracted/AL_DEFINITIONS.json")) {
        fprintf(stderr, "Warning: Failed to load main definitions\n");
        /* Continue anyway */
    }
    
    /* Load short word definitions */
    if (!load_definitions("/Users/jasonwalsh/projects/xscrabble-v3/data/dictionaries/extracted/AL_SHORT_DEFINITIONS.json")) {
        fprintf(stderr, "Warning: Failed to load short word definitions\n");
        /* Continue anyway */
    }

    printf("Loaded %d words and %d definitions\n\n", word_count, entry_count);

    /* Process command-line arguments */
    if (argc > 1) {
        if (strcmp(argv[1], "-i") == 0) {
            /* Interactive mode */
            char command[32];
            char param[64];
            
            printf("Interactive mode. Commands:\n");
            printf("  lookup WORD - Look up a word's definition\n");
            printf("  list - List all words\n");
            printf("  quiz NUM - Take a quiz with NUM questions\n");
            printf("  search PATTERN - Search for words containing PATTERN\n");
            printf("  quit - Exit the program\n\n");
            
            while (1) {
                printf("> ");
                if (scanf("%31s", command) != 1) break;
                
                if (strcmp(command, "quit") == 0) {
                    break;
                }
                else if (strcmp(command, "lookup") == 0) {
                    if (scanf("%63s", param) != 1) continue;
                    print_word_info(param);
                }
                else if (strcmp(command, "list") == 0) {
                    list_all_words();
                }
                else if (strcmp(command, "quiz") == 0) {
                    int num = 5; /* Default */
                    scanf("%d", &num);
                    random_quiz(num);
                }
                else if (strcmp(command, "search") == 0) {
                    if (scanf("%63s", param) != 1) continue;
                    search_by_pattern(param);
                }
                else {
                    printf("Unknown command. Try 'lookup', 'list', 'quiz', 'search', or 'quit'.\n");
                }
                
                /* Clear input buffer */
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }
        }
        else {
            /* Look up a specific word */
            print_word_info(argv[1]);
        }
    }
    else {
        /* Display sample words */
        printf("Sample words from the AL dictionary:\n\n");
        
        /* Display 5 random entries with definitions */
        srand(time(NULL));
        for (int i = 0; i < 5 && i < entry_count; i++) {
            int idx = rand() % entry_count;
            print_word_info(dictionary[idx].word);
        }
        
        printf("\nRun with -i for interactive mode\n");
    }

    cleanup();
    return 0;
}

/* Load word list from text file */
bool load_word_list(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return false;
    }
    
    /* Allocate memory for word list if not already allocated */
    if (!word_list) {
        word_list = (char **)malloc(MAX_ENTRIES * sizeof(char *));
        if (!word_list) {
            fclose(file);
            return false;
        }
    }
    
    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, MAX_LINE_LENGTH, file) && word_count < MAX_ENTRIES) {
        /* Remove newline */
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        /* Skip empty lines and comments */
        if (len == 0 || buffer[0] == '#') continue;
        
        /* Add to word list */
        word_list[word_count] = strdup(buffer);
        word_count++;
    }
    
    fclose(file);
    return true;
}

/* Simple JSON parsing for the definitions file */
bool load_definitions(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return false;
    }
    
    /* Read entire file into a buffer */
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char *json_data = (char *)malloc(file_size + 1);
    if (!json_data) {
        fclose(file);
        return false;
    }
    
    size_t bytes_read = fread(json_data, 1, file_size, file);
    json_data[bytes_read] = '\0';
    fclose(file);
    
    /* Very simplistic JSON parsing - this is just a demo */
    /* In a real application, use a proper JSON parser like cJSON */
    char *ptr = json_data;
    
    /* Skip opening brace */
    if (*ptr != '{') {
        free(json_data);
        return false;
    }
    ptr++;
    
    while (*ptr && entry_count < MAX_ENTRIES) {
        /* Find next word key */
        ptr = strstr(ptr, "\"");
        if (!ptr) break;
        
        /* Extract word */
        char *word_start = ptr + 1;
        char *word_end = strstr(word_start, "\"");
        if (!word_end) break;
        
        size_t word_len = word_end - word_start;
        if (word_len >= sizeof(dictionary[entry_count].word)) {
            word_len = sizeof(dictionary[entry_count].word) - 1;
        }
        
        strncpy(dictionary[entry_count].word, word_start, word_len);
        dictionary[entry_count].word[word_len] = '\0';
        
        /* Find definition */
        ptr = strstr(word_end, "\"definition\"");
        if (!ptr) break;
        
        ptr = strstr(ptr, ":");
        if (!ptr) break;
        
        ptr = strstr(ptr, "\"");
        if (!ptr) break;
        
        char *def_start = ptr + 1;
        char *def_end = strstr(def_start, "\"");
        if (!def_end) break;
        
        size_t def_len = def_end - def_start;
        dictionary[entry_count].definition = (char *)malloc(def_len + 1);
        strncpy(dictionary[entry_count].definition, def_start, def_len);
        dictionary[entry_count].definition[def_len] = '\0';
        
        /* Find English definition */
        ptr = strstr(def_end, "\"english_definition\"");
        if (ptr) {
            ptr = strstr(ptr, ":");
            if (ptr) {
                ptr = strstr(ptr, "\"");
                if (ptr) {
                    char *eng_start = ptr + 1;
                    char *eng_end = strstr(eng_start, "\"");
                    if (eng_end) {
                        size_t eng_len = eng_end - eng_start;
                        dictionary[entry_count].english_definition = (char *)malloc(eng_len + 1);
                        strncpy(dictionary[entry_count].english_definition, eng_start, eng_len);
                        dictionary[entry_count].english_definition[eng_len] = '\0';
                        ptr = eng_end;
                    }
                }
            }
        } else {
            dictionary[entry_count].english_definition = NULL;
        }
        
        /* We'll skip spanish_definition for now, but it's in the JSON file for future use */
        
        /* Find example */
        ptr = strstr(def_end, "\"example\"");
        if (ptr) {
            ptr = strstr(ptr, ":");
            if (ptr) {
                ptr = strstr(ptr, "\"");
                if (ptr) {
                    char *ex_start = ptr + 1;
                    char *ex_end = strstr(ex_start, "\"");
                    if (ex_end) {
                        size_t ex_len = ex_end - ex_start;
                        dictionary[entry_count].example = (char *)malloc(ex_len + 1);
                        strncpy(dictionary[entry_count].example, ex_start, ex_len);
                        dictionary[entry_count].example[ex_len] = '\0';
                        ptr = ex_end;
                    }
                }
            }
        } else {
            dictionary[entry_count].example = NULL;
        }
        
        /* Find part of speech */
        ptr = strstr(def_end, "\"part_of_speech\"");
        if (ptr) {
            ptr = strstr(ptr, ":");
            if (ptr) {
                ptr = strstr(ptr, "\"");
                if (ptr) {
                    char *pos_start = ptr + 1;
                    char *pos_end = strstr(pos_start, "\"");
                    if (pos_end) {
                        size_t pos_len = pos_end - pos_start;
                        dictionary[entry_count].part_of_speech = (char *)malloc(pos_len + 1);
                        strncpy(dictionary[entry_count].part_of_speech, pos_start, pos_len);
                        dictionary[entry_count].part_of_speech[pos_len] = '\0';
                        ptr = pos_end;
                    }
                }
            }
        } else {
            dictionary[entry_count].part_of_speech = NULL;
        }
        
        entry_count++;
        
        /* Move to next entry */
        ptr = strstr(ptr, "}");
        if (!ptr) break;
        ptr++;
    }
    
    free(json_data);
    return entry_count > 0;
}

/* Clean up resources */
void cleanup(void) {
    if (dictionary) {
        for (int i = 0; i < entry_count; i++) {
            if (dictionary[i].definition) free(dictionary[i].definition);
            if (dictionary[i].english_definition) free(dictionary[i].english_definition);
            if (dictionary[i].example) free(dictionary[i].example);
            if (dictionary[i].part_of_speech) free(dictionary[i].part_of_speech);
        }
        free(dictionary);
    }
    
    if (word_list) {
        for (int i = 0; i < word_count; i++) {
            free(word_list[i]);
        }
        free(word_list);
    }
}

/* Display information about a word */
void print_word_info(const char *word) {
    /* Convert to uppercase for comparison */
    char uppercase[32];
    strncpy(uppercase, word, sizeof(uppercase) - 1);
    uppercase[sizeof(uppercase) - 1] = '\0';
    
    for (int i = 0; uppercase[i]; i++) {
        uppercase[i] = toupper(uppercase[i]);
    }
    
    /* First check if the word is in our dictionary with definitions */
    const ALDictionaryEntry *entry = lookup_word(uppercase);
    
    if (entry) {
        printf("=== %s ===\n", entry->word);
        printf("Definition (FR): %s\n", entry->definition);
        
        if (entry->english_definition) {
            printf("Definition (EN): %s\n", entry->english_definition);
        }
        
        if (entry->part_of_speech) {
            printf("Part of speech: %s\n", entry->part_of_speech);
        }
        
        if (entry->example) {
            printf("Example: %s\n", entry->example);
        }
        
        printf("\n");
        return;
    }
    
    /* If not in the dictionary with definitions, check the word list */
    for (int i = 0; i < word_count; i++) {
        if (strcmp(word_list[i], uppercase) == 0) {
            printf("=== %s ===\n", uppercase);
            printf("This word is in the AL dictionary but has no definition yet.\n");
            printf("You can add a definition to learn more about it!\n\n");
            return;
        }
    }
    
    printf("Word '%s' not found in the AL dictionary.\n\n", uppercase);
}

/* Look up a word in the dictionary */
const ALDictionaryEntry* lookup_word(const char *word) {
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(dictionary[i].word, word) == 0) {
            return &dictionary[i];
        }
    }
    return NULL;
}

/* List all words */
void list_all_words(void) {
    printf("AL Dictionary Words (%d total):\n", word_count);
    printf("-----------------------------\n");
    
    int cols = 5;
    int col_width = 12;
    
    for (int i = 0; i < word_count; i++) {
        printf("%-*s", col_width, word_list[i]);
        if ((i + 1) % cols == 0) {
            printf("\n");
        }
    }
    
    if (word_count % cols != 0) {
        printf("\n");
    }
    printf("\n");
}

/* Quiz the user on random words */
void random_quiz(int num_questions) {
    if (entry_count == 0) {
        printf("No definitions available for quiz.\n");
        return;
    }
    
    if (num_questions > entry_count) {
        num_questions = entry_count;
    }
    
    printf("French Learning Quiz (%d questions)\n", num_questions);
    printf("----------------------------------\n\n");
    
    int correct = 0;
    char answer[64];
    
    for (int i = 0; i < num_questions; i++) {
        int idx = rand() % entry_count;
        const ALDictionaryEntry *entry = &dictionary[idx];
        
        /* Determine quiz type (1=FR->EN, 2=EN->FR, 3=Example) */
        int quiz_type = (rand() % 3) + 1;
        
        if (quiz_type == 1 && entry->english_definition) {
            /* French to English */
            printf("Q%d: What does \"%s\" mean in English?\n", i+1, entry->word);
            printf("Definition (FR): %s\n", entry->definition);
            printf("Your answer: ");
            
            /* Clear input buffer */
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            
            if (fgets(answer, sizeof(answer), stdin)) {
                /* Remove newline */
                size_t len = strlen(answer);
                if (len > 0 && answer[len - 1] == '\n') {
                    answer[len - 1] = '\0';
                }
                
                printf("Correct answer: %s\n", entry->english_definition);
                /* Very simple answer checking - in a real app, use better matching */
                if (strstr(entry->english_definition, answer) || strstr(answer, entry->english_definition)) {
                    printf("Correct!\n\n");
                    correct++;
                } else {
                    printf("Not quite. Keep learning!\n\n");
                }
            }
        }
        else if (quiz_type == 2 && entry->english_definition) {
            /* English to French */
            printf("Q%d: What is the AL dictionary word for \"%s\"?\n", i+1, entry->english_definition);
            printf("Your answer: ");
            
            /* Clear input buffer */
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            
            if (fgets(answer, sizeof(answer), stdin)) {
                /* Remove newline */
                size_t len = strlen(answer);
                if (len > 0 && answer[len - 1] == '\n') {
                    answer[len - 1] = '\0';
                }
                
                /* Convert to uppercase for comparison */
                for (int j = 0; answer[j]; j++) {
                    answer[j] = toupper(answer[j]);
                }
                
                printf("Correct answer: %s\n", entry->word);
                if (strcmp(answer, entry->word) == 0) {
                    printf("Correct!\n\n");
                    correct++;
                } else {
                    printf("Not quite. Keep learning!\n\n");
                }
            }
        }
        else if (quiz_type == 3 && entry->example) {
            /* Fill in the blank */
            /* Replace the word in the example with blanks */
            char example_with_blank[256];
            strcpy(example_with_blank, entry->example);
            
            /* Create blank string of same length as word */
            char blank[32];
            size_t word_len = strlen(entry->word);
            for (int j = 0; j < word_len; j++) {
                blank[j] = '_';
            }
            blank[word_len] = '\0';
            
            /* Find word in example and replace with blank */
            char *word_pos = strstr(example_with_blank, entry->word);
            if (word_pos) {
                strncpy(word_pos, blank, word_len);
            }
            
            printf("Q%d: Fill in the blank with the correct word:\n", i+1);
            printf("%s\n", example_with_blank);
            if (entry->definition) {
                printf("Hint: %s\n", entry->definition);
            }
            printf("Your answer: ");
            
            /* Clear input buffer */
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            
            if (fgets(answer, sizeof(answer), stdin)) {
                /* Remove newline */
                size_t len = strlen(answer);
                if (len > 0 && answer[len - 1] == '\n') {
                    answer[len - 1] = '\0';
                }
                
                /* Convert to uppercase for comparison */
                for (int j = 0; answer[j]; j++) {
                    answer[j] = toupper(answer[j]);
                }
                
                printf("Correct answer: %s\n", entry->word);
                if (strcmp(answer, entry->word) == 0) {
                    printf("Correct!\n\n");
                    correct++;
                } else {
                    printf("Not quite. Keep learning!\n\n");
                }
            }
        }
        else {
            /* Fallback quiz type */
            printf("Q%d: What is the meaning of \"%s\"?\n", i+1, entry->word);
            printf("Your answer: ");
            
            /* Clear input buffer */
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            
            if (fgets(answer, sizeof(answer), stdin)) {
                /* Remove newline */
                size_t len = strlen(answer);
                if (len > 0 && answer[len - 1] == '\n') {
                    answer[len - 1] = '\0';
                }
                
                printf("Meaning: %s\n", entry->definition);
                /* Very simple answer checking */
                if (strstr(entry->definition, answer) || strstr(answer, entry->definition)) {
                    printf("Correct!\n\n");
                    correct++;
                } else {
                    printf("Not quite. Keep learning!\n\n");
                }
            }
        }
    }
    
    printf("Quiz complete! You got %d out of %d correct.\n\n", correct, num_questions);
}

/* Search for words containing a pattern */
void search_by_pattern(const char *pattern) {
    char uppercase_pattern[64];
    strncpy(uppercase_pattern, pattern, sizeof(uppercase_pattern) - 1);
    uppercase_pattern[sizeof(uppercase_pattern) - 1] = '\0';
    
    for (int i = 0; uppercase_pattern[i]; i++) {
        uppercase_pattern[i] = toupper(uppercase_pattern[i]);
    }
    
    printf("Words containing '%s':\n", uppercase_pattern);
    printf("-------------------------\n");
    
    int count = 0;
    for (int i = 0; i < word_count; i++) {
        if (strstr(word_list[i], uppercase_pattern)) {
            printf("%s\n", word_list[i]);
            count++;
        }
    }
    
    if (count == 0) {
        printf("No matches found.\n");
    } else {
        printf("\n%d matches found.\n", count);
    }
    printf("\n");
}