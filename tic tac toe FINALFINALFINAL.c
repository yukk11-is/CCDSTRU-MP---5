#include <stdio.h>
#include <Windows.h>

#define ROWS 3
#define COLS 3

void printTitle(){

    printf("_____ _                 _____                     _____            _ \n");
    printf("|_   _(_)               |_   _|                   |_   _|          | |\n");
    printf("  | |  _  ___   ______    | | __ _  ___   ______    | | ___   ___  | |\n");
    printf("  | | | |/ __| |______|   | |/ _` |/ __| |______|   | |/ _ \\ / _ \\ | |\n");
    printf("  | | | | (__             | | (_| | (__             | | (_) |  __/ |_|\n");
    printf("  \\_/ |_|\\___|            \\_/\\__,_|\\___|            \\_/\\___/ \\___| (_)\n");

printf("\n\n--------------------------------------------------------------\n\n");


}

void printVal (char array[ROWS] [COLS]){
	
	int r = 0;
	int c = 0;
	
	printf("-------------\n");
	for (r = 0; r < ROWS; r++){
		printf("|");
		for (c = 0; c < COLS; c++){
			printf(" %c |", array[r][c]);
			}
		printf("\n-------------\n");
		}
	}

void makeMove(char array[ROWS] [COLS], int row, int column, int playerO, int playerX){
	
	char cValue;
	if (playerO == 1){
		cValue = 'O';
	} else if(playerX == 1){
		cValue = 'X';
	}
	
	array [row] [column] = cValue;
}

void selectCoor(char array[ROWS] [COLS], int *playerO, int *playerX){
	
	int row;
	int column;
	int nValidator1 = 0;
	int nValidator2 = 0;
	int nValidator3 = 0;
	
	printf("\n-----------------------------------\n");

	
do{	
		if (*playerO == 1){
		printf("You are player O\n\n");
	} else if (*playerX == 1){
		printf("You are player X\n\n");
	}
	
	printf("Make your move:\n");
	printf("Row: 1-3 only\n");

	do {
	scanf("%d" , &row);	
		if ( row > 3 || row < 0 ){
			nValidator1 = 0;
			printf("\nPick a valid number bruh.\n");
			printf("%d \n", nValidator1);
		}else if (row <= 3 && row >= 1){
			nValidator1 = 1;
			printf("%d \n", nValidator1);
		}
	} while ( nValidator1 == 0 );
	
	printf("Column: 1-3 only\n");
	
	do 	{
	scanf("%d", &column);
		if ( column > 3 || column < 0){
			nValidator2 = 0;
			printf("\nPick a valid number bruh.\n");
		}else if ( column <= 3 && column >= 1 ){
			nValidator2 = 1;
		}
	} while ( nValidator2 == 0);
	printf("Row: %d  Column: %d\n", row, column);
	
	
	if (array[row - 1][column - 1] == '-'){
		nValidator3 = 1;
		makeMove (array, row - 1, column - 1, *playerO, *playerX);	
	} else if (array[row - 1][column - 1] != '-'){
		printf("\nThat square is already taken. Pick again.\n");
		nValidator3 = 0;
		sleep(2);
	}
} while ( nValidator3 == 0 );
}

	
//	makeMove (array, row - 1, column - 1, *playerO, *playerX);


void switchPlayer (int *playerX, int *playerO){
	
	if(*playerX == 0){
		*playerX = 1;
	}else if(*playerX == 1){
		*playerX = 0;
	}
	if(*playerO == 0){
		*playerO = 1;
	}else if(*playerO == 1){
		*playerO = 0;
	}
}

void finalChecker (char array[ROWS][COLS], int *gameStatus){
	int i = 0;
	int j = 0;
	int winFound = 0;
	
	for ( i; i < ROWS && !winFound; i++ ){
			if (array[i][j] != '-' && array[i][0] == array[i][1] && array[i][2] == array[i][1]){
				*gameStatus = 1;
				printf("The winner is %c, Congratulations!\n", array[i][0]);
				printVal(array);
				winFound = 1;
				sleep(1);
			}
	}
	
	i = 0;
	j = 0;
	
	for ( j; j < COLS && !winFound; j++ ){
			if (array[0][j] != '-' && array[1][j] == array[0][j] && array[2][j] == array[1][j]){
				*gameStatus = 1;
				printf("The winner is %c, Congratulations!\n", array[0][j]);
				printVal(array);
				winFound = 1;
				sleep(1);
			}
	}
	i = 0;
	j = 0;
			if (array[0][0]!= '-' && array[0][0] == array[1][1] && array[1][1] == array[2][2]){
				*gameStatus = 1;
				printf("The winner is %c, Congratulations!\n", array[0][0]);
				printVal(array);
				winFound = 1;
				sleep(1);
			}
	i = 0;
	j = 0;

			if (array[0][2] != '-' && array[0][2] == array[1][1] && array[1][1] == array[2][0]){
				*gameStatus = 1;
				printf("The winner is %c, Congratulations!\n", array[0][2]);
				printVal(array);
				winFound = 1;
				sleep(1);
			}
	
}
int main(){

	char nArray [ROWS] [COLS] = {
	{'-', '-', '-'}, 
	{'-', '-', '-'}, 
	{'-', '-', '-'}};
	int rowCoor;
	int columnCoor;
	int X = 0;
	int O = 1;
	
	int gameStatus = 0;
	int *playerX = &X;
	int *playerO = &O;
	
	printTitle();
	
	while (gameStatus == 0){
		printVal (nArray);
		selectCoor (nArray, playerO, playerX);
		switchPlayer (playerX, playerO);
		finalChecker (nArray, &gameStatus);
		sleep(1);
		system("cls");
	}
	
/*
TO - DO : 

- You need to do the terms for winning and shit.
- Make sure that if the square is already occupied that you can't put that in anymore.
- Make sure that the input cannot be greater than 3 and what not (Put an if statement and an input checker).
- Display names for the title, the winning and the losing.
- Use srand, the one you used in the gla to create vs ai.

*/
	return 0;
}