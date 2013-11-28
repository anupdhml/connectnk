CFLAGS = -Wall --std=c99 -pedantic -g
R_OBJS = replay.o board.o board_play.o
C_OBJS = connect.o board.o board_play.o
OBJS = connectnk-full.o board.o board_play.o

all: replay connect connectnk-full


# OBJS

replay.o:  replay.c board.h board.c board_play.h board_play.c
	gcc -c $(CFLAGS) -o replay.o replay.c

connect.o:  connect.c board.h board.c board_play.h board_play.c
	gcc -c $(CFLAGS) -o connect.o connect.c

connectnk-full.o:  connectnk-full.c board.h board.c board_play.h board_play.c
	gcc -c $(CFLAGS) -o connectnk-full.o connectnk-full.c

board.o:  board.c board.h
	gcc -c $(CFLAGS) -o board.o board.c

board_play.o:  board_play.c board.c board.h board_play.h
	gcc -c $(CFLAGS) -o board_play.o board_play.c


# Three programs

replay: $(R_OBJS)
	gcc -o replay $(R_OBJS)

connect: $(C_OBJS)
	gcc -o connect $(C_OBJS)

connectnk-full: $(OBJS)
	gcc -o connectnk-full $(OBJS)


# Miscellany

clean: 
	rm -f replay connect connectnk-full $(OBJS) $(R_OBJS) $(C_OBJS) $(DOC)
