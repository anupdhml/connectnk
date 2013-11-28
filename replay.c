/*
 * ============================================================================
 * replay.c
 * Reads the replay file for the connectnk game.
 *
 * <anupdhml@gmail.com>, 12/04/11
 * =============================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "board.h"
#include "board_play.h"

static void argparse (int argc, char *argv[], FILE **inputfile);

// ----------------------------------------------------------------------------------

int main (int argc, char *argv[]) {
    Board *b;
    FILE *input_file = NULL;

    argparse(argc, argv, &input_file);
    
    /* system("clear"); */
    
    b = board_load(input_file);
    printf("\n\nYou have used a replay file! Press enter to see the next move,\n" 
           "<ctrl-d> to exit.\n");
    
    // Now replay the game...
    
    Result game_result;
    bool player2_turn = false;
    
    game_result = board_play(b, input_file, &player2_turn);
    board_print_final(b, game_result, player2_turn);
    
    fclose(input_file);
    board_destroy(b); 

    return EXIT_SUCCESS;
}

// ----------------------------------------------------------------------------------

// Check if filenames were provided as arguments.
static void argparse (int argc, char *argv[], FILE **inputfile) {
    if (argc == 1) {
        fprintf(stderr, "replay: No replay file provided.\n");
        exit(1);
    }
    
    // will read the last valid file from the given list
    for (int i = 1; i < argc; i++) {
        *inputfile = fopen(argv[i], "r");
        if (*inputfile == NULL) {
            fprintf(stderr, "replay: %s: No such file\n", argv[i]); 
            exit(2); 
        }

    }
}
