#include <stdio.h>
#include <string.h>

typedef struct coordTag coord; //if you want to declare a coordinate variable use

struct coordTag{ //stores x and y coordinates as structs for easier record keeping
	int x;	  //If you guys dont want structs we can use 2d arrays 
	int y;
};

typedef struct{
	
/*
	NOTE: the arrays below do not hold arrays, they hold boolean (0,1) that says if 
	coordinate belongs to the set. ALSO we will not be following the row-major attribute
	of the C language, so that the array indices and cartesian coordinates.
	^^^^^^^^^^^^^^^^^^^^^^^^^^PLEASE TAKE NOTE OF THIS^^^^^^^^^^^^^^^^^^^^^^^^^^
	
	Example:
	Is coordinate (2,3) found in set B?
	B[2][3] = 0; 2 is the x coord, 3 is the y coord and, 0 is saying that the coord is not In B
	
*/
	
	//R,B,S,T are all initialized as NULL
	int R [4][4]; //Coordinates owned by R
	int B [4][4]; //Coordinates owned by B
	int S [4][4]; //Coordinates selected
	int T [4][4]; //Coordinates touched
	
	int cntF; // number of FREE unclaimed spaces
	int cntR; //cardinality of R
	int cntB; //cardinality of B
	int cntS; //cardinality of S
	int cntT; //cardinality of T
	
	int good;
    int go;
    int start;
    int found;
    int val;
    
}gameState;

/*
NOTE: I have initially set all function types to void, 
this may change depending on the requirements later on
*/

void Remove (coord *pos){
	
}

void Replace(coord *pos){
	
}

void Expand (coord *pos){
	
}

void Update(coord *pos){
	
}

void NextPlayerMove (coord *pos){
	
}

int GameOver(gameState *status){
	
	return -888;
}

int checkGame(gameState status){
	if((status.cntF==3||
	   status.val>=20||
	   status.start==0)&&
	   ((status.cntR>0&&status.cntB==0)&&
	   (status.cntB>0&&status.cntR==0))){
	   	return 0;
	   }
	return 1;
}

void ZeroArray (int array[][4]){
	int i,j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++)
			array[i][j] = 0;
	}
}


int main() {
//NOTE since set V is T or F, we will use int {1,0} (Binary)
gameState status;

//------------SYSTEM INITIALIZATION------------
//NOTE since set V is T or F, we will use int {1,0} (Binary)
status.good = 0;
status.go = 1;
status.start = 1;
status.found = 0;

status.val = 0;
status.cntF = 9; //9 remaining FREE spaces in start of game

//Initialize R,B,S,T as NULL
ZeroArray(status.R);
ZeroArray(status.B);
ZeroArray(status.S);
ZeroArray(status.T);


coord pos; //This is NOT part of the list of variables from MP specs


while (checkGame(status)){ //Game Loop Control
	
}
  	  

	return 0;
}

