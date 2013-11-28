/*
 * ============================================================================
 * board.h
 * Header file for the basic board data structure
 *
 * <anupdhml@gmail.com>, 12/04/11
 * =============================================================================
 */

#ifndef BOARD_H
#define BOARD_H

typedef struct board Board;
struct board {
    int width;          // No of columns
    int height;         // No of rows
    int win_length;     // Length of st. line for win
    char *state;        // The state of the board with all the entries
    int full_columns;   // Total no of full columns
};

#define LINE1_LENGTH 100  // For reading the first line of replay file

Board *board_new(int width, int height, int win_length);
Board *board_load (FILE *input_file);
void board_draw (Board *b);
void board_destroy(Board *b);


/* Professor Balckom's example, slightly modified to allow multiplication factors*/
#ifndef _MACROS_H
#define _MACROS_H

//#include <stdio.h>
//#include <stdlib.h>

#define FATAL(s) { fprintf(stderr, "EXITING:  Fatal error at [%s:%d]: %s\n", __FILE__, __LINE__, s); exit(EXIT_FAILURE); }

#define NEW(s,t,w) if(((s) = (t *) malloc(w * sizeof(t))) == NULL) { FATAL("malloc() returned NULL"); }

#endif 
/* end of macro */

#endif 
