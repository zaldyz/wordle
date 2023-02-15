// Wordle C Program side project 10/5/2022
// By Zaldy Thalib

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define WORD_LENGTH 5
#define MAX_STRING 256

void red(void);
void green(void);
void yellow(void);
void cyan(void);
void reset(void);
void string_to_upper(char *src);
bool string_is_alpha(char *src);
bool process_guess(char *guess, char *answer);
void set_false(bool* src, int len);
void print_guess(char *guess, bool *guess_yellow, bool *guess_green);
void print_welcome_msg(void);
void get_word(char *answer);
long wordcount(char *filename);

int main(void) {

    print_welcome_msg();
    char *answer = malloc(sizeof(char) * (WORD_LENGTH + 1));
    char *guess = malloc(sizeof(char) * (WORD_LENGTH + 1));
    
    // Choose a word from wordlist.txt
    get_word(answer);

    // Do the guessing stage now
    int num_guesses = 0;
    bool correct = false;

    while (num_guesses < 6 && !correct) {
        char *buffer = malloc(sizeof(char) * MAX_STRING);
        printf("Enter your %d-letter guess: ", WORD_LENGTH);
        scanf("%s", buffer);
        if (strlen(buffer) != WORD_LENGTH) {
            red();
            printf("Invalid Guess: Must be %d-letter word\n", WORD_LENGTH);
            reset();
        } else if (!string_is_alpha(buffer)) {
            red();
            printf("Invalid Guess: Contains non-alpha characters\n");
            reset();
        } else {
            strcpy(guess, buffer);
            string_to_upper(guess);
            // Process the guess here
            if (process_guess(guess, answer)) {
                correct = true;
            }
            num_guesses++;
        }
        free(buffer);
    }

    if (correct) {
        green();
        if (num_guesses == 1) {
            printf("Congratulations!, you found the word in %d guess.\n", num_guesses);
        } else {
            printf("Congratulations!, you found the word in %d guesses.\n", num_guesses);
        }
        reset();
    } else {
        cyan();
        printf("Subhuman... the word was %s\n", answer);
        reset();
    }

    free(answer);
    free(guess);
    return 0;
}


// Sets text colour to bold, red
void red(void) {
    printf("\033[1;31m");
}
// Sets text colour to bold, green
void green(void) {
    printf("\033[1;32m");
}
// Sets text colour to bold, yellow
void yellow(void) {
    printf("\033[1;33m");
}
// Sets text colour to bold, cyan
void cyan(void) {
    printf("\033[1;36m");
}
// Resets text to default
void reset(void) {
    printf("\033[0m");
}

// Converts a string to uppercase
void string_to_upper(char *src) {
    for (int i = 0; src[i] != '\0'; i++) {
        src[i] = toupper((unsigned char) src[i]);
    }
}

// Checks whether a string contains all alphabet characters
bool string_is_alpha(char *src) {
    for (int i = 0; src[i] != '\0'; i++) {
        if (!isalpha((unsigned char) src[i])) {
            return false;
        }
    }
    return true;
}

// Process a guess, print the appropriate colours
// Print in the following format
// ===========
// G U E S S
// 🟨⬜️⬜️⬜️🟩
// ===========
bool process_guess(char *guess, char *answer) {
    bool *guess_yellow = malloc(sizeof(bool) * WORD_LENGTH);
    bool *guess_green = malloc(sizeof(bool) * WORD_LENGTH);
    bool *accounted = malloc(sizeof(bool) * WORD_LENGTH);
    set_false(guess_yellow, WORD_LENGTH);
    set_false(guess_green, WORD_LENGTH);
    set_false(accounted, WORD_LENGTH);

    // For each letter in GUESS
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] == answer[i]) {
            // then it is green
            guess_green[i] = true;
            accounted[i] = true;
        } else {
            // it is not green, so we check if it is yellow
            for (int j = 0; j < WORD_LENGTH; j++) {
                if (j != i && guess[i] == answer[j]) {
                    // check if it is not going to be green and it is not accounted
                    if (guess[j] != answer[j] && !accounted[j]) {
                        accounted[j] = true;
                        guess_yellow[i] = true;
                    }
                }
            }
        }
    }

    // Print in the correct format
    print_guess(guess, guess_yellow, guess_green);
    bool correct = true;
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess_green[i]) {
            // Green
            printf("🟩");
        } else if (guess_yellow[i]) {
            // Yellow
            printf("🟨");
            correct = false;
        } else {
            // White
            printf("⬜️");
            correct = false;
        }
    }   
    printf("\n");
    free(guess_yellow);
    free(guess_green);
    free(accounted);
    return correct;
}

// Sets a boolean array to false
void set_false(bool* src, int len) {
    for (int i = 0; i < len; i++) {
        src[i] = false;
    }
}

// Prints guess in one letter spacing
void print_guess(char *guess, bool *guess_yellow, bool *guess_green) {
    for (int i = 0; guess[i] != '\0'; i++) {
        if (guess_yellow[i]) {
            yellow();
        } else if (guess_green[i]) {
            green();
        }
        printf("%c ", guess[i]);
        reset();
    }
    printf("\n");
}  

// Prints the welcome message
void print_welcome_msg(void) {
    cyan();
    printf("=======================\n");
    printf("||  Wordle By Zaldy  ||\n");
    printf("||                   ||\n");
    printf("||  CTRL + C to exit ||\n");
    printf("=======================\n");
    reset();
}

// Function which chooses a random word from wordlist.txt to be the answer
void get_word(char *answer) {
    // seed
    srand(time(NULL));
    long numwords = wordcount("wordlist.txt");
    long r = (long)rand() % numwords;
    long byte_offset = (r * (WORD_LENGTH + 1));
    
    FILE *fp = fopen("wordlist.txt", "r");
    if (fp == NULL) {
        perror("wordlist.txt");
        exit(1);
    }
    char buffer[MAX_STRING];
    fseek(fp, byte_offset, SEEK_SET);
    fscanf(fp, "%s", buffer);
    strcpy(answer, buffer);
    string_to_upper(answer);
}

// Returns the number of lines in a file
long wordcount(char *filename) {

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error reading file %s\n", filename);
        perror(filename);
    }
    long numlines = 0;
    int ch = fgetc(fp);
    while (ch != EOF) {
        if (ch == '\n') {
            // New line
            numlines++;
        }
        ch = fgetc(fp);
    }
    fclose(fp);
    return numlines;
}


/*
    Planning:
    5 Letter word, allocate space for an array to store the actual word,
    Allocate space for two 5 length arrays answer_green and answer_yellow which 
    contain a boolean whether or not that square is green or yellow for a guess.
    Allocate a third array, accounted, which is a boolean whether or not that position
    has been used to create another yellow or not.
    reset the arrays after each guess. 

    🟩🟨⬜️

    ANSWER = CHEWY

    GUESS = STARE
               S T A R E
    Expected = ⬜️⬜️⬜️⬜️🟨

    ANSWER = TRAIN
    GUESS =  BRAIR
               B R A I R 
    Expected = ⬜️🟩🟩🟩⬜️

    ANSWER = TRANN
    GUESS =  NNOIN
               N N O I N 
    Expected = 🟨⬜️⬜️⬜️🟩

    here accounted[3] should be set to 1 so that the second N doesnt become yellow.



    For each letter in the GUESS, first check if it is green, if it is not,
    loop through the entire answer and see if there are any matches of that letter
    somewhere else in the word, if a match is found, check if that position is a green
    if it is a green, move on. If we find a match that isnt green, and isnt 'accounted',
    We make it yellow and set the matching position to be accounted. 
    If we finish checking all positions and find no 
    matches or no non-green matches, make it white.

*/
