/*
 * ============================================================================
 * connectnk-full.c
 * Combined version of the two programs replay and connect, with some added
 * improvements.
 *
 * conneck n-k: a generalised version of the connect four.
 * Use -c switch to set custom width, height and win combination, in that order.
 * Reads the replay file until someone has won. If no one wins at the end of
 * file, you may continue the game.
 * Also, while reading from a replay file, press the EOF marker to key to start
 * playing...
 *
 * <anupdhml@gmail.com>, 12/04/11
 * =============================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "board.h"
#include "board_play.h"


static void argparse (int argc, char *argv[], bool *do_replay, bool *do_custom, 
                        int *width, int *height, int *win_length, FILE **inputfile);

// ----------------------------------------------------------------------------------

int main (int argc, char *argv[]) {
    
    // Replay feature, and custom input options for board parameters
    bool do_replay, do_custom;
    do_replay = do_custom = false;

    Board *b;
    int width, height, win_length;
    width = 7; height = 6; win_length = 4;  // Default board
    FILE *input_file = NULL;

    argparse(argc, argv, &do_replay, &do_custom, &width, &height, &win_length, &input_file);
   
    // The play begins...
    
    Result game_result = PLAY_START;   // Required for playing the replay file midway
    bool player2_turn = false;
    
    /* system("clear"); */
    
    if (do_replay) {
        b = board_load(input_file);
        
        printf("\n\nYou have used a replay file! Press enter to see the next move,\n" 
               "<ctrl-d> to play the game from the current state.\n");
        
        game_result = board_play(b, input_file, &player2_turn);
        fclose(input_file);
    }
    
    else
        b = board_new(width, height, win_length);
   
    // Either use the fresh board or start playing from the replay file.
    
    /* system("clear"); */
    
    printf("\n\nPlay the game by entering the column no. Valid input range: %d-%d\n"
            "<ctrl-d> exits the game.\n", 0, b->width -1);
    
    if (game_result == PLAY_START)
        game_result = board_play (b, stdin, &player2_turn);
    
    board_print_final(b, game_result, player2_turn);
   
    board_destroy(b); 

    return EXIT_SUCCESS;
}

// ----------------------------------------------------------------------------------

// Parse the arguments 
static void argparse (int argc, char *argv[], bool *do_replay, bool *do_custom, 
                        int *width, int *height, int *win_length, FILE **inputfile) {
  
    for (int i = 1; i < argc; i++) {
    
        // Check for the game parameters provided
        if (strcmp(argv[i], "-c") == 0) {
            if (i + 3 <= argc - 1) {    // Check if 3 arguments follow the switch -c.

                if ( isnum(argv[i+1]) && isnum(argv[i+2]) && isnum(argv[i+3]) ) {
                    *do_custom = true;
                    i++;
                    *width = atoi(argv[i]);  // Writing the value string as int
                    i++;
                    *height = atoi(argv[i]);
                    i++;
                    *win_length = atoi(argv[i]);

                    if( (*width <= 0) || (*height <= 0) || (*win_length <= 0) ) {
                        fprintf(stderr, "connectnk: Invalid argument provided for the switch -c."
                                        "All the parameters must be greater than zero\n");
                        exit(22);
                    }
                    
                    if (*win_length > *width || *win_length > *height) {
                        fprintf(stderr, "connectnk: Invalid argument provided for the switch -c."
                                        "Win length can't exceed the number of columns or rows\n");
                        exit(22);
                    }

                }
                else { 
                    fprintf(stderr, "connectnk: Invalid argument provided for the switch -c. " 
                                    "Only positive integers are allowed.\n"); 
                    exit(22);
                }
            }  

            else {
                fprintf(stderr, "connectnk: Three arguments required for the switch -c\n"); 
                exit(22);
            }
        }
    
        // Check if filenames were provided as arguments.
        else { 
            *inputfile = fopen(argv[i], "r");
            if (*inputfile == NULL) {
                fprintf(stderr, "connectnk: %s: No such file\n", argv[i]); 
                exit(2); 
            }
            else {
                *do_replay = true;
            } 
        }
    }
}
