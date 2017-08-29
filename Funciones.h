#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

typedef struct {

	int * rows;
	int * columns;
	int * mines;
	int ** matrix;
} Board;

typedef struct {

	int * rows;
	int * columns;
	int markedMines;
	int ** matrix;
} UI;

typedef struct {

	int * rows;
	int * columns;
	int ** matrix;
} StatesBoard;

void clear();

void decreaseMarkedMines(UI * ui, int row, int column);

Board * createBoard ();

void loseBoard (Board * board, UI * ui);

void fillBoard (Board *board, UI *ui, int row, int column, char play);

void setClues (Board *board);

void saveBoard (Board *board);

void expandSpaces (Board * board, UI * ui, StatesBoard * states, int row, int column);

void showUI (Board *board, UI *ui);

int countSpaces (StatesBoard * states) ;

int nextPlay (Board * board, UI * ui, StatesBoard * states, int safeSpace) ;

void run ();