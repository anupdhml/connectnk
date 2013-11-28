/*
 * ============================================================================
 * board.c
 * A basic board data structure
 * 
 * <anupdhml@gmail.com>, 14/04/11
 * =============================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "board.h"

/* ============================================================================= */

// Constructor for board
Board *board_new(int width, int height, int win_length) {
    Board *b;
    NEW(b, Board, 1);   // multiply sizeof Board by 1, useful in other cases
    /* b = (Board *) malloc(sizeof(Board)); */
    
    b->width = width;               // No of rows
    b->height = height;             // No of columns
    b->win_length = win_length;     // Length of line needed for a win
    b->full_columns = 0;            // No of full columns at the start
    
    int bsize = width * height;     // Size of board
    char *init_state;

    /* init_state = (char *) malloc(bsize * sizeof(char)); */
    NEW(init_state, char, bsize);
    
    for (int i = 0; i < bsize; i++ )
        init_state[i] = '.';

    b->state = init_state;          // State of the board, with all the entries

    return b;
}

/* ============================================================================= */

// Read from the replay file to set the board parameters
Board *board_load (FILE *input_file) {
    
    char buffer[LINE1_LENGTH];   // for reading the first line
    
    // Check if the first 7 characters form the word REPLAY
    fgets (buffer, 7, input_file);
    if ( strcmp(buffer, "REPLAY") != 0 ) {
        fprintf(stderr, "connectnk: The file provided is not a replay file.\n");
        exit(1);
    }
    
    // Set the board parameters
    int replay_win_length, replay_width, replay_height;
    replay_height = 0;
    
    fgets (buffer, LINE1_LENGTH, input_file);
    int chars_read = sscanf(buffer, "%d %d %d", &replay_win_length, &replay_width, &replay_height);
    if (chars_read < 2) {
        fprintf(stderr, "connectnk: Invalid replay file. Board parameters are not specified correctly\n");
        exit(1);
    }

    // In case of only two parameters, set height the same as width.
    if((replay_height) == 0)
        replay_height = replay_width;

    Board *b = board_new(replay_width, replay_height, replay_win_length);
    
    return b;
}

/* ============================================================================= */

// Draw the board in all its glory
void board_draw (Board *b) {
    int bsize = (b->width) * (b->height);
    char hole;
    int i;
    
    printf("\n============================================\n"
           "connectnk game with %d columns, %d rows.     \n" 
           , b->width, b->height);
    
    for (i = 0; i < bsize; i++ ) {
        hole = (b->state)[i];
        
        if (i % (b->width) == 0)  // Time to start the new row
            printf("\n");
        printf("%c ", hole);
    }

    printf("\n\nA straight line of length %d required to win.\n", b->win_length);
    printf("============================================\n\n");

}

/* ============================================================================= */

// Free the memory assosciated with the board
void board_destroy(Board *b) {
    free(b->state);
    free(b);
}

/* ============================================================================= */
