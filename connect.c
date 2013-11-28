/*
 * ============================================================================
 * connect.c
 * A two player connectnk game. Takes 3 arguments: no of columns, no of rows and
 * the length required to win.
 *
 * <anupdhml@gmail.com>, 12/04/11
 * =============================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "board.h"
#include "board_play.h"

static void argparse (int argc, char *argv[], 
                      int *width, int *height, int *win_length);

// ----------------------------------------------------------------------------------

int main (int argc, char *argv[]) {
    Board *b;
    int width, height, win_length;
    width = 7; height = 6; win_length = 4;  // Default board

    argparse(argc, argv, &width, &height, &win_length);
   
    /* system("clear"); */
  
    b = board_new(width, height, win_length);
    printf("\n\nPlay the game by entering the column no. Valid input range: %d-%d\n"
            "<ctrl-d> exits the game.\n", 0, b->width -1);
    
    // Now play the game...
    
    Result game_result;
    bool player2_turn = false;
    
    game_result = board_play (b, stdin, &player2_turn);
    board_print_final(b, game_result, player2_turn);
   
    board_destroy(b); 

    return EXIT_SUCCESS;
}

// ----------------------------------------------------------------------------------

// Check for the game parameters provided
void argparse (int argc, char *argv[], int *width, int *height, int *win_length) {
    for (int i = 1; i < argc; i++) {
        
        if (i + 2 <= argc - 1) {    // Check if 3 arguments follow the switch -c.
            if ( isnum(argv[i]) && isnum(argv[i+1]) && isnum(argv[i+2]) ) {
                *width = atoi(argv[i]);  // Writing the value string as int
                i++;
                *height = atoi(argv[i]);
                i++;
                *win_length = atoi(argv[i]);

                if( (*width <= 0) || (*height <= 0) || (*win_length <= 0) ) {
                    fprintf(stderr, "connect: Invalid argument provided. "
                                    "All the parameters must be greater than zero\n");
                    exit(22);        // Proper unix error code...
                }
                
                if (*win_length > *width || *win_length > *height) {
                    fprintf(stderr, "connect: Invalid argument provided. "
                                    "Win length can't exceed the number of columns or rows\n");
                    exit(22);
                }

            }
            
            else { 
                fprintf(stderr, "connect: Invalid argument provided. " 
                                "Only positive integers are allowed.\n"); 
                exit(22);
            }
        }  
        
        else {
            fprintf(stderr, "connect: Three arguments required.\n"); 
            exit(22);
        }

    }
}
