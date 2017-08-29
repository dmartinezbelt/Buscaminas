#include "Funciones.h"

void clear(){
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}

void decreaseMarkedMines(UI * ui, int row, int column) {

	if (ui->matrix[row][column] == 2) {
		
		ui->markedMines--;
	}
}

Board * createBoard () {

	Board *board;
	board = (Board*)malloc(sizeof(Board));

	int i,j;
	char *rows, *columns, *mines;
	board = (Board*)malloc(sizeof(Board));
	board->rows = (int*)malloc(sizeof(int));
	board->columns = (int*)malloc(sizeof(int));
	board->mines = (int*)malloc(sizeof(int));
	rows = (char*)malloc(sizeof(char)*10);
	columns = (char*)malloc(sizeof(char)*10);
	mines = (char*)malloc(sizeof(char)*10);
	
	while (1) {

		printf("Ingrese la cantidad de filas: ");
		scanf("%s", rows);
		*board->rows = atoi(rows);

		if (*board->rows > 1) {
			break;
		} else {

			printf("Debe ser un número mayor que 1\n");
		}
	}

	while (1) {

		printf("Ingrese la cantidad de columnas: ");
		scanf("%s", columns);
		*board->columns = atoi(columns);

		if (*board->columns > 1 && *board->columns <=100) {

			break;
		} else {

			printf("El número de columnas debe ser entre 2 y 100\n");
		}
	}

	while (1) {

		printf("Ingrese la cantidad de minas: ");
		scanf("%s", mines);
		*board->mines = atoi(mines);

		if (*board->mines > 0 && *board->mines < ((*board->rows)*(*board->columns))) {

			break;
		} else {

			printf("La cantidad de minas debe ser entre 1 y %d\n", ((*board->rows)*(*board->columns) - 1));
		}
	}
	printf("\n");
	board->matrix = (int**)malloc(sizeof(int*)*(*board->rows));

	for (i = 0; i < *board->rows; i++) {

		board->matrix[i] = (int*)malloc(sizeof(int)*(*board->columns));
		}
	for (i = 0; i < (*board->rows); i++) {

		for (j = 0; j < (*board->columns); j++) {

			board->matrix[i][j] = 0;
		}
	}
	rows = NULL;
	columns = NULL;
	mines = NULL;
	free(rows);
	free(columns);
	free(mines);
	clear();
	return board;
}

void loseBoard (Board * board, UI * ui) {

	int i,j;

	for (i = 0; i < *ui->rows; i++) {
			
		for (j = 0; j < *ui->columns; j++) {

			if (board->matrix[i][j] == 9) {

				ui->matrix[i][j] = 1;
			}
		}
	}
}

void fillBoard (Board *board, UI *ui, int row, int column, char play) {

	int i,j;
	int randomRow;
	int randomColumn;
	int mineCount = 0;
	srand(time(NULL));
	ui->matrix[row][column] = 1;

	while (mineCount < *board->mines) {
		
		randomRow = rand() % *board->rows;
		randomColumn = rand() % *board->columns;
		
		if ((randomRow != row || randomColumn != column) && (board->matrix[randomRow][randomColumn] != 9) && (play == 'O')) {

			board->matrix[randomRow][randomColumn] = 9;
			mineCount++;
		}
	}
}

void setClues (Board *board) {

	int i,j;

	for (i = 0; i < *board->rows; i++)	{

		for (j = 0; j < *board->columns; j++)	{

			if (board->matrix[i][j] != 9) {

				if (i - 1 >= 0 && board->matrix[i-1][j] == 9) {

					board->matrix[i][j]++;
				}
				if (i - 1 >= 0 && j - 1 >= 0 && board->matrix[i-1][j-1] == 9) {

					board->matrix[i][j]++;
				}
				if (j - 1 >= 0 && board->matrix[i][j-1] == 9) {

					board->matrix[i][j]++;
				}
				if (i + 1 < *board->rows && j - 1 >= 0 && board->matrix[i+1][j-1] == 9) {

					board->matrix[i][j]++;
				}
				if (i + 1 < *board->rows && board->matrix[i+1][j] == 9) {

					board->matrix[i][j]++;
				}
				if (i + 1 < *board->rows && j + 1 < *board->columns && board->matrix[i+1][j+1] == 9) {

					board->matrix[i][j]++;
				}
				if (j + 1 < *board->columns && board->matrix[i][j+1] == 9) {

					board->matrix[i][j]++;
				}
				if (i - 1 >= 0 && j + 1 < *board->columns && board->matrix[i-1][j+1] == 9) {

					board->matrix[i][j]++;
				}
			}
		}
	}
}

void saveBoard (Board *board) {

	int i,j;
	char EMPTY = '.';
	char MINE = '*';
	FILE * file = fopen("Solucion.out", "w");
	fprintf(file, "'*'= Mina\n'.'= Espacio Vacío\n");
	for (int i = 0; i < *board->rows; i++) {

		for (int j = 0; j < *board->columns; j++) {

			if (board->matrix[i][j] == 0) {

				fprintf(file, "%2c", EMPTY);
			} else if (board->matrix[i][j] == 9) {

				fprintf(file, "%2c", MINE);
			} else {

				fprintf(file, "%2d", board->matrix[i][j]);
			}
		}
		fprintf(file,"\n");
	}
	fclose(file);
}

void expandSpaces (Board * board, UI * ui, StatesBoard * states, int row, int column) {

	decreaseMarkedMines(ui, row, column);
	if (states->matrix[row][column] != 1) {
		
		states->matrix[row][column] = 1;
		ui->matrix[row][column] = 1;
	}
	if (row - 1 >= 0 && board->matrix[row-1][column] == 0 && states->matrix[row-1][column] == 0) {

	expandSpaces(board, ui, states, row - 1, column);
	} else if (row - 1 >= 0 && board->matrix[row-1][column] != 9 && states->matrix[row-1][column] == 0) {
		
		decreaseMarkedMines(ui, row-1, column);
		ui->matrix[row-1][column] = 1;
		states->matrix[row-1][column] = 1;
	}
	if (row - 1 >= 0 && column - 1 >= 0 && board->matrix[row-1][column-1] == 0 && states->matrix[row-1][column-1] == 0) {

		expandSpaces(board, ui, states, row - 1, column - 1);
	} else if (row - 1 >= 0 && column - 1 >= 0 && board->matrix[row-1][column-1] != 9 && states->matrix[row-1][column-1] == 0) {
	
		decreaseMarkedMines(ui, row-1, column-1);
		ui->matrix[row-1][column-1] = 1;
		states->matrix[row-1][column-1] = 1;
	}
	if (column - 1 >= 0 && board->matrix[row][column-1] == 0 && states->matrix[row][column-1] == 0) {

		expandSpaces(board, ui, states, row, column - 1);
	} else if (column - 1 >= 0 && board->matrix[row][column-1] != 9 && states->matrix[row][column-1] == 0) {

		decreaseMarkedMines(ui, row, column-1);
		ui->matrix[row][column-1] = 1;
		states->matrix[row][column-1] = 1;
	}
	if (row + 1 < *board->rows && column - 1 >= 0 && board->matrix[row+1][column-1] == 0 && states->matrix[row+1][column-1] == 0) {

		expandSpaces(board, ui, states, row + 1, column - 1);
	} else if (row + 1 < *board->rows && column - 1 >= 0 && board->matrix[row+1][column-1] != 9 && states->matrix[row+1][column-1] == 0) {

		decreaseMarkedMines(ui, row+1, column-1);
		ui->matrix[row+1][column-1] = 1;
		states->matrix[row+1][column-1] = 1;
	}
	if (row + 1 < *board->rows && board->matrix[row+1][column] == 0 && states->matrix[row+1][column] == 0) {

		expandSpaces(board, ui, states, row + 1, column);
	} else if (row + 1 < *board->rows && board->matrix[row+1][column] != 9 && states->matrix[row+1][column] == 0) {

		decreaseMarkedMines(ui, row+1, column);
		ui->matrix[row+1][column] = 1;
		states->matrix[row+1][column] = 1;
	}
	if (row + 1 < *board->rows && column + 1 < *board->columns && board->matrix[row+1][column+1] == 0 && states->matrix[row+1][column+1] == 0) {

		expandSpaces(board, ui, states, row + 1, column + 1);
	} else if (row + 1 < *board->rows && column + 1 < *board->columns && board->matrix[row+1][column+1] != 9 && states->matrix[row+1][column+1] == 0) {

		decreaseMarkedMines(ui, row+1, column+1);
		ui->matrix[row+1][column+1] = 1;
		states->matrix[row+1][column+1] = 1;
	}
	if (column + 1 < *board->columns && board->matrix[row][column+1] == 0 && states->matrix[row][column+1] == 0) {

		expandSpaces(board, ui, states, row, column + 1);
	} else if (column + 1 < *board->columns && board->matrix[row][column+1] != 9 && states->matrix[row][column+1] == 0) {

		decreaseMarkedMines(ui, row, column+1);
		ui->matrix[row][column+1] = 1;
		states->matrix[row][column+1] = 1;
	}
	if (row - 1 >= 0 && column + 1 < *board->columns && board->matrix[row-1][column+1] == 0 && states->matrix[row-1][column+1] == 0) {

		expandSpaces(board, ui, states, row - 1, column + 1);
	} else if (row - 1 >= 0 && column + 1 < *board->columns && board->matrix[row-1][column+1] != 9 && states->matrix[row-1][column+1] == 0) {

		decreaseMarkedMines(ui, row-1, column+1);
		ui->matrix[row-1][column+1] = 1;
		states->matrix[row-1][column+1] = 1;
	}
}

void showUI (Board *board, UI *ui) {

	int i,j;
	char CLOSED = '_';
	char MARKED = 'X';
	char EMPTY = '.';
	char MINE = '*';
	char BLANK = ' ';

	for (i = 0; i < *ui->rows + 2; i++) {
		
		for (j = 0; j < *ui->columns + 2; j++) {

			if (i == 0) {

				if (*board->columns <= 10) {

					if (j == 0) {

						printf("%2c", BLANK);
					} else if (j == 1) {

						printf("|");
					} else {

						printf("%2d", j - 2);
					}
				} else {

					if (j == 0) {

						printf("%3c", BLANK);
					} else if (j == 1) {

						printf("|");
					} else {

						printf("%3d", j - 2);
					}
				}
			} else if (i == 1) {

				if (*board->columns <= 10) {

					if (j == 0) {

						printf("__");
					} else if (j == 1) {

						printf("|");
					} else {

						printf("__");
					}
				} else {	

					if (j == 0) {

						printf("___");
					} else if (j == 1) {

						printf("|");
					} else {

						printf("___");
					}
				}
			} else {

				if (*board->columns <= 10) {

					if (j == 0) {

						printf("%2d", i - 2);
					} else if (j == 1) { 

						printf("|");
					} else {

						if (ui->matrix[i-2][j-2] == 0) {

							printf("%2c", CLOSED);
						} else if (ui->matrix[i-2][j-2] == 1) {

							if (board->matrix[i-2][j-2] == 0) {
								
								printf("%2c", EMPTY);
							} else if(board->matrix[i-2][j-2] != 9) {

								printf("%2d", board->matrix[i-2][j-2]);
							} else {
								printf("%2c", MINE);
							}
						} else {

							printf("%2c", MARKED);
						}
					}
				} else {

					if (j == 0) {

						printf("%3d", i - 2);
					} else if (j == 1) { 

						printf("|");
					} else {

						if (ui->matrix[i-2][j-2] == 0) {

							printf("%3c", CLOSED);
						} else if (ui->matrix[i-2][j-2] == 1) {

							if (board->matrix[i-2][j-2] == 0) {
								
								printf("%3c", EMPTY);
							} else if(board->matrix[i-2][j-2] != 9) {

								printf("%3d", board->matrix[i-2][j-2]);
							} else {
								printf("%3c", MINE);
							}
						} else {

							printf("%3c", MARKED);
						}
					}
				}
			}
		}
		printf("\n");
	}
	printf("Minas Totales = %d | Minas Marcadas = %d", *board->mines, ui->markedMines);
}

int countSpaces (StatesBoard * states) {

	int i,j,count;
	count = 0;
	for (i = 0; i < *states->rows; i++) {
		
		for (j = 0; j < *states->columns; j++) {	

			count = count + states->matrix[i][j];
		}
	}
	return count;
}

int nextPlay (Board * board, UI * ui, StatesBoard * states, int safeSpace) {

	int row, column, count;
	int win = 0;
	char play;
	char *row2, *column2;
	row2 = (char*)malloc(sizeof(char)*10);
	column2 =(char*)malloc(sizeof(char)*10);
	while (1) {

		int randomRow = rand() % *board->rows;
		int randomColumn = rand() % *board->columns;

		showUI(board, ui);
		printf("\nIngrese una Jugada (Ejemplo: %d %d X): ", randomRow, randomColumn);
		scanf("%s %s %c", row2, column2, &play);
		row = atoi(row2);
		column = atoi(column2);
		play = toupper(play);

		if ((row >= 0 && row < *board->rows) && (column >= 0 && column < *board->columns) && play == 'O' && states->matrix[row][column] != 1) {

			clear();
			printf("Ultima Jugada: %d %d %c", row, column, play);
			break;
		} else if ((row >= 0 && row < *board->rows) && (column >= 0 && column < *board->columns) && play == 'X' && ui->matrix[row][column] == 0) {

			clear();
			printf("Ultima Jugada: %d %d %c", row, column, play);
			printf("\n");
			ui->matrix[row][column] = 2;
			ui->markedMines++;
		} else if ((row >= 0 && row < *board->rows) && (column >= 0 && column < *board->columns) && play == 'X' && ui->matrix[row][column] == 2) {

			clear();printf("Ultima Jugada: %d %d %c", row, column, play);
			printf("\n");
			ui->matrix[row][column] = 0;
			ui->markedMines--;
		} else {

			clear();
			printf("Alguno de los datos de la jugada es erróneo, vuelva a ingresarlos\n");
			printf("Ultima Jugada: %d %d %c", row, column, play);
			printf("\n");
		}
	}

	if (play == 'O') {

		decreaseMarkedMines(ui, row, column);

		if (board->matrix[row][column] == 0) {
			
			expandSpaces(board, ui, states, row, column);
		} else {

			ui->matrix[row][column] = 1;
			states->matrix[row][column] = 1;
		}
		count = countSpaces(states);
		printf("\n");

		if (count == safeSpace) {

			win = 1;
		}
		if (board->matrix[row][column] == 9) {
			
			loseBoard(board,ui);
			win = 2;
		}
	}
	row2 = NULL;
	column2 = NULL;
	free(row2);
	free(column2);
	return win;
}

void run () {

	Board *board;
	UI *ui;
	StatesBoard *states;
	char play;
	int i,j,row,column,safeSpace;
	char * row2, * column2;
	int win = 0;
	int count = 0;

	row2 = (char*)malloc(sizeof(char)*10);
	column2 = (char*)malloc(sizeof(char)*10);
	clear();
	board = createBoard();
	safeSpace = (*board->rows) * (*board->columns) - *board->mines;

	ui = (UI*)malloc(sizeof(UI));
	ui->rows = board->rows;
	ui->columns = board->columns;

	ui->matrix = (int**)malloc(sizeof(int*)*(*ui->rows));

	for (i = 0; i < *ui->rows; ++i) {

		ui->matrix[i] = (int*)malloc(sizeof(int)*(*ui->columns));
	}
	for (i = 0; i < *ui->rows; i++)	{

		for (j = 0; j < *ui->columns; j++) {

			ui->matrix[i][j] = 0;
		}
	}
	states = (StatesBoard*)malloc(sizeof(StatesBoard));
	states->rows = board->rows;
	states->columns = board->columns;
	states->matrix = (int**)malloc(sizeof(int*)*(*states->rows));

	for (i = 0; i < *states->rows; ++i) {

		states->matrix[i] = (int*)malloc(sizeof(int)*(*states->columns));
	}
	for (i = 0; i < *states->rows; i++)	{

		for (j = 0; j < *states->columns; j++) {

			states->matrix[i][j] = 0;
		}
	}
	while (1) {

		int randomRow = rand() % *board->rows;
		int randomColumn = rand() % *board->columns;

		showUI(board, ui);
		printf("\nIngrese una Jugada (Ejemplo: %d %d X): ", randomRow, randomColumn);
		scanf("%s %s %c", row2, column2, &play);
		row = atoi(row2);
		column = atoi(column2);
		play = toupper(play);

		if ((row >= 0 && row < *board->rows) && (column >= 0 && column < *board->columns) && play == 'O') {

			clear();
			printf("Ultima Jugada: %d %d %c", row, column, play);
			break;
		} else if ((row >= 0 && row < *board->rows) && (column >= 0 && column < *board->columns) && play == 'X' && ui->matrix[row][column] == 0) {

			clear();
			printf("Ultima Jugada: %d %d %c", row, column, play);
			printf("\n");
			ui->matrix[row][column] = 2;
			ui->markedMines++;
		} else if ((row >= 0 && row < *board->rows) && (column >= 0 && column < *board->columns) && play == 'X' && ui->matrix[row][column] == 2) {

			clear();
			printf("Ultima Jugada: %d %d %c", row, column, play);
			printf("\n");
			ui->matrix[row][column] = 0;
			ui->markedMines--;
		} else {

			clear();
			printf("Alguno de los datos de la jugada es erróneo, vuelva a ingresarlos\n");
			printf("Ultima Jugada: %d %d %c", row, column, play);
			printf("\n");
		}
	}
	if (play == 'O') {

		fillBoard(board, ui, row, column, play);
		setClues(board);
		saveBoard (board);
		decreaseMarkedMines(ui, row, column);

		if (board->matrix[row][column] == 0) {
			
			expandSpaces(board, ui, states, row, column);
		} else {

			states->matrix[row][column] = 1;
		}
		count = countSpaces(states);
		printf("\n");

		if (count == safeSpace) {

			win = 1;
		}
	}
	if (win != 1) {

		while (1) {

			win = nextPlay(board, ui, states, safeSpace);
			if (win != 0) {

				break;
			}
		}
	}
	if (win == 1) {

		showUI(board, ui);
		printf("\n¡Felicidades! Ha ganado la partida\n");
	} else {

		showUI(board, ui);
		printf("\n¡¡¡BOOM!!! Usted ha perdido\n");
	}
	printf("Presione la tecla Enter");
	getchar();
	getchar();

	for (int i = 0; i < *ui->rows; i++) {

		board->matrix[i] = NULL;
		ui->matrix[i] = NULL;
		states->matrix[i] = NULL;
		free(board->matrix[i]);
		free(ui->matrix[i]);
		free(states->matrix[i]);
	}
	board->matrix = NULL;
	ui->matrix = NULL;
	states->matrix = NULL;
	free(board->matrix);
	free(ui->matrix);
	free(states->matrix);
	board = NULL;
	ui = NULL;
	states = NULL;
	row2 = NULL;
	column2 = NULL;
	free(board);
	free(ui);
	free(states);
	free(row2);
	free(column2);
}