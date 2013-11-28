# Connectnk Game

Connect-k-n: A generalization of [Connect Four](http://en.wikipedia.org/wiki/Connect_Four) where k is the number of pieces needed in a line to win, and n is size of the square board. Thus, standard connect-four is conenct-4-7.

## The three programs

connect: For a two player connectnk game. Takes three arguments: width (no of columns), height (no of rows) and the length of the straight line needed for a win.

replay: Reads a REPLAY file to step through the each move of a previous game

connectnk-full: Just something that combines the above two features. Also allows playing of the REPLAY file at any time. (Use the -c switch to specify custom board parameters.)


## Overview of the program design

`replay.c` and `connect.c` use a basic board data structure to store the data for the game and handle the playing tasks. This data structure and various functions associated with it are defined in the files `board.c` and `board_play.c`

The programs are similar in structure and they even use the same function for the playing and replaying. The major difference between the two is that of different input sources: for *replay*, it is some given file, while for *connect*, it is stdin. All other differences are direct consequences of this. Thus, even though the two programs seem to be written for different purposes, they are essentially the same in the overall design of the program. 

Here are the steps the two programs take, which reflect the common design of the programs:

* Using a private *argparse* function to determine the validity of the arguments provided when the programs were started. 

* Creating a board based on the input width, height and win_length. 

* Determining the result by playing (or replaying) the game, based on the column inputs or something that the player did during the game (did he decide to quit?). 

* Printing the end result based on the result

* Clean-up tasks (destroy the board from the memory, close the file if it was opened.) before exiting.

Just to further demonstrate the similarity between the two programs, a separate program *connectnk-full* was written, with some added improvements.


## The board data structure

Functions to create, draw and destroy the board are in the `board.c` file while `board_play.c` handles tasks related to the actual playing one turn at a time (namely, getting the input column, checking if the move resulted in a concrete result and if there was a result, announcing it.) Division was done in this manner to mirror how players would approach a real-life connectnk game. Assuming that the players are sufficiently well-off to buy a board (or maybe they are lucky enough to own one, perhaps it was handed down to them by previous generations), they never have to worry about actually creating the board. They can just grab one and start playing. 

### Notes on some of the functions

* *board_load* function in `board.c` just reads the first line of the input file, and at the end calls *board_new* (the constructor) to actually set the board parameters. Thus, in the main of the *replay* program, *board_load* behaves like the constructor for the board when the source is a file.

* In *board_get_input* of `board_play.c`, counting for column input starts from zero. For stdin input, this is a bit incovenient, but it was done to make the function compatible for both REPLAY file input and stdin input, and thus keep things as simple as possible.  

* *board_play* of `board_play.c` is the central function combining all other major functions in the file, except for *board_print_final*. This print task could have been done inside this central function and it would have in fact saved a few lines in the *connect* and *replay* programs -- at the moment, the `Result` variable in the main of these two programs might seem a bit superfluous. But the variable plays an important role in *connectnk-full*, by allowing the player to play the REPLAY file any time the player wishes too. *connectnk-full* can thus use *board_play* two times, each time with a different input source. Thus, it would make sense to print the 'final' message outside *board_play*. 


## Header files

`board.h` and `board_play.h` are just header files associated with the above two files. They contain prototypes for the major functions of these two files. They also define constants and use typedefs to make the code more readable. For example, `board_play.c` has a type `Result`, which is actually an `int`. Under the hood, it takes different integer values to denote the various states of gameplay: play start, unresolved, win, draw and quit. 

