/*
 * ============================================================================
 * board_play.h
 * For the manipulation of the board data structure during playing (and
 * replaying)
 *
 * <anupdhml@gmail.com>, 12/04/11
 * =============================================================================
 */


#ifndef BOARD_PLAY_H
#define BOARD_PLAY_H

// Game outcomes
typedef int Result;

#define UNRESOLVED 0
#define WIN 1
#define DRAW 2
#define PLAY_START -1
#define QUIT -2

// For the entries at the prompt and board
#define EOF_ENTRY -2
#define INVALID_ENTRY -1

// For reading the column input
#define BUFFERSIZE 50

int board_get_input (Board *b, FILE *source, bool *second_player);
int board_move (Board *b, int column_no, bool *second_player);
Result board_check_win (Board *b, int curr_index);
Result board_play(Board *board, FILE *source, bool *second_player);
void board_print_final (Board *b, Result game_result, bool second_player); 

bool isnum(char arg[]);  // helper function

#endif 
