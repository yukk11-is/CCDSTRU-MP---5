#include <stdio.h>
#include <string.h>

typedef struct coord coord;

struct coord{ //stores x and y coordinates as structs for easier record keeping
	int x;	  //If you guys dont want structs we can use 2d arrays 
	int y;
};

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

void GameOver(){
	
}


int main() {
//NOTE since set V is T or F, we will use int {1,0} (Binary)

int good = 0,
	go = 0, 
	start = 1, 
	found = 0; //Initialization of boolean int variables
	 
int val = 0; // initialization of non-boolean int variables

coord r[9] = {}, // There are 9 coordinates in a 3x3 grid NOTE: coord is a struct
	  b[9] = {}, 
	  s[9] = {}, 
	  t[9] = {}, 
  	  f[9] = {}; // initialization of int arrays that will hold coordinates
  	
coord pos; //This is NOT part of the list of variables from MP specs
  	  

	return 0;
}

