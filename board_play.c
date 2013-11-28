/*
 * ============================================================================
 * board_play.c
 * Functions related to the manipulation of board data structure while playing
 * (and replaying).
 *
 * <anupdhml@gmail.com>, 17/04/11
 * =============================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "board.h"
#include "board_play.h"

// To handle the various tests while checking for a win
enum test {
    horz_left, horz_right, diag_down_left, diag_down_right,
    diag_up_left, diag_up_right, vert_down
} line;   

static int find_adj_index(int curr_index, enum test line, Board *b);
static bool is_row_start (int index, int total_columns);

/* ============================================================================= */

// Play (or replay) the board game and return the result
Result board_play(Board *b, FILE *source, bool *player2_turn) {
    int entry_index; 
    Result game_result;
    
    do {
       board_draw(b);                                                                                                                                      
       
       // the board index where the last read input goes
       entry_index = board_get_input(b, source, player2_turn);
       
       game_result = board_check_win (b, entry_index);

       if (game_result == QUIT) {     // You decided to quit
           game_result = PLAY_START;  // Necessary to continue playing the replay file
           break;                     // in connect-full
       }
    }
    while (game_result == UNRESOLVED);  // Continue playing the game..
    
    return game_result;
}

/* ============================================================================= */

// Get the column input to change the board
int board_get_input (Board *b, FILE *source, bool *player2_turn) {
    
    char input[BUFFERSIZE];
    int curr_index;  // Input goes to this index in the board
    
    if (!(*player2_turn))   
        printf("Player 1 will move now: "); 
    else
        printf("Player 2 will move now: "); 
    
    // Read the input source (either file or stdin) for column no
    // Reading the no of chars in the contol string should be enough. 
    // input should hold all of it, as BUFFERSIZE is usually a high number
    // Could have used Professor Balckom's readstring example for a better
    // approach.
    if ( fscanf(source, "%20s", input) == EOF ) { 
        curr_index = EOF_ENTRY; 
        return curr_index;    // enables exit if EOF is encountered in input
    }
    
    // While reading from files, pause for the enter key naviagtion
    if(source != stdin) {
        char c;
        while ( (c = fgetc(stdin)) != '\n' ) {
            if (c == EOF) {
                curr_index = EOF_ENTRY;
                return curr_index; // enables keyboard exit when files are the source
            }
        }
    }
   
    /* system("clear"); */
    
    // Check the validity of input and if valid, make the board move.
    // Then set the board index based on the input. Counting starts from zero
    if (isnum(input) && atoi(input) < b->width) {
        int column_input = atoi(input);  // converting from the string
        curr_index = board_move(b, column_input, player2_turn);
    }
    else {
        fprintf(stderr, "\nInvalid entry. "                                                                           
                "Only numbers between 0 and %d are allowed.\n", b->width - 1);
        curr_index = INVALID_ENTRY;  // later enables re-asking of input
    }

    return curr_index;
}

/* ============================================================================= */

// Alter the board state when a move is made. 
// Returns the board index where the input goes.
int board_move (Board *b, int column_no, bool *player2_turn) {
    int row_no = b->height - 1;     // The last row
    int total_columns = b->width;
    bool is_column_full = false;   // at start, assume the column is not full
    
    int i = column_no + (row_no * total_columns); // index, related to board parameters 

    // Determine if the column is full or not
    while ( (b->state)[i] != '.') {
        if (i < total_columns) {
            fprintf(stderr, "\nThe entered column %d was full." 
                            "Please enter another column no.\n", column_no);
            is_column_full = true;
            break;
        }
        
        i = i - total_columns;
    }

    // Only when the column is not full, changes can be made
    if (!is_column_full) {
        if (i < total_columns) 
            ++(b->full_columns); // increase the total no of full columns
        
        if (!(*player2_turn))
            (b->state)[i] = 'o';
        else 
            (b->state)[i] = '*';
    
        *player2_turn = !(*player2_turn);  // flip the turn
    
    }

    return i; 
}

/* ============================================================================= */

// Check the board state for a win. curr_index is assosciated with the most
// recent input. (from file as well as stdin)
Result board_check_win (Board *b, int curr_index) {
    int i;
    
    // End of file or user exit from stdin 
    if (curr_index == EOF_ENTRY)
        return QUIT;        
    
    // Not a proper column input
    if (curr_index == INVALID_ENTRY)
        return UNRESOLVED;      // so that we can continue to play   

    // Only now try the various tests to see if the recent move was a winning one
    for (line = horz_left; line <= vert_down; line++) {
        for (i = 1; i <= (b->win_length); i++) {
            
            // No need to try other tests if the current test was successful
            if(i == b->win_length)
                return WIN;
            
            // the adjacent index to the current one
            int adj_index = find_adj_index(curr_index, line, b);
            
            if (adj_index == INVALID_ENTRY)  // No adjacent elment present
                break;

            // if the adj index entry matches, continue the current test 
            if(b->state[curr_index] == b->state[adj_index])
                curr_index = adj_index;
            else
                break; // Try the next test now
        }
    }
    
    if(b->full_columns == b->width)  // Check for draw
        return DRAW;

    return UNRESOLVED;  // All the tests were done and no test was positive
}

/* ============================================================================= */

// Print the final messages based on the game_result
void board_print_final (Board *b, Result game_result, bool player2_turn) {
    /* system("clear"); */
    
    if (game_result == WIN) {                                                                                                                                          if (player2_turn)
            printf("\n\nPlayer 1 has won!\n");
        else
            printf("\n\nPlayer 2 has won!\n");
    }
    
    else if (game_result == DRAW) {
        printf("\n\nMatch was a draw. Good game!\n"); 
    } 
    
    else { 
        printf("\n\nMatch was left unresolved either because you decided to leave, \n"
               "or because the replay file did not yield a win (if replay was used).\n"); 
    } 
    
    printf("Here's the last seen footage of the board.\n");
    board_draw(b);
}

/* ============================================================================= */

/* HELPER FUNCTIONS BEGIN HERE */

/* ============================================================================= */

// Returns true if the given string is numeric, false otherwise. Used in argparse too..
bool isnum(char arg[]) {
    int i;
  
    for (i = 0; i < strlen(arg); i++)
        if (!isdigit(arg[i])) {
         return false;
         break;
        }

    return true;
  
    /* if( atoi(arg) > 0)  // convert the arg string to num first */
        /* return 1; */
    /* else */
        /* return 0; */
}


// Find the adjacent index to the given index in the line specified
// If such an index does not exist, return INVALID_ENTRY.
static int find_adj_index(int curr_index, enum test line, Board *b) {
    int adj_index;
    int total_columns = b->width;
    int highest_index = (b->height * b->width) - 1; 
    
    switch(line) {
        
        case horz_left:
            adj_index = curr_index - 1;
            if ( is_row_start(curr_index, total_columns) )
                adj_index = INVALID_ENTRY; 
            break;
        
        case horz_right:
            adj_index = curr_index + 1;
            if ( is_row_start(adj_index, total_columns) )
                adj_index = INVALID_ENTRY;
            break;
        
        case diag_down_left:
            adj_index = (curr_index + total_columns) - 1;
            if ( adj_index > highest_index || is_row_start(curr_index, total_columns) )
                adj_index = INVALID_ENTRY;
            break;
        
        case diag_down_right:
            adj_index = (curr_index + total_columns) + 1;
            if ( adj_index > highest_index || is_row_start(adj_index, total_columns) )
                adj_index = INVALID_ENTRY;
            break;
        
        case diag_up_left:
            adj_index = (curr_index - total_columns) - 1;
            if ( adj_index < 0 || is_row_start(curr_index, total_columns) )
                adj_index = INVALID_ENTRY;
            break;
        
        case diag_up_right:
            adj_index = (curr_index - total_columns) + 1;
            if ( adj_index < 0 || is_row_start(adj_index, total_columns) )
                adj_index = INVALID_ENTRY;
            break;
        
        case vert_down:
            adj_index = curr_index + total_columns;
            if (adj_index > highest_index)
                adj_index = INVALID_ENTRY;
            break;
    }

    return adj_index;
}

// to determine if the given board index lies at the beginning of a row
static bool is_row_start (int index, int total_columns) {
    return ( (index % total_columns) == 0 );
}

/* ============================================================================= */
