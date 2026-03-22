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

//Function prototypes
int checkValid(coord pos);
int hasCoord(int array[][4],coord pos);
void Expand (gameState *status, coord *pos);
int checkGame(gameState status);

/*
NOTE: I have initially set all function types to void, 
this may change depending on the requirements later on
*/

void Remove (gameState *status, coord *pos){
if(checkValid(*pos)){
	if(status->go==1){ //Remove pos from set R
		status->R[pos->x][pos->y] = 0;
		status->cntR--;
			
	}else{// Remove pos from set B
		status->B[pos->x][pos->y] = 0;
		status->cntB--;
	}
		
	//Remove pos from S
	status->S[pos->x][pos->y] = 0;
	status->cntS--;
		
	//Remove pos from T
	status->T[pos->x][pos->y] = 0;
	status->cntT--;
}
}

void Replace(gameState *status, coord *pos){
	status->found = 0;
	
	if((status->go==1)&&(hasCoord(status->B,*pos))){
		status->B[pos->x][pos->y] = 0;
		status->cntB--;
		status->found = 1;
	}
	if((status->go==1)&&(hasCoord(status->R,*pos))){
		status->found = 1;
	}
	if((status->go==1)&&!(hasCoord(status->R,*pos))){
		status->R[pos->x][pos->y] = 1;
		status->cntR++;
	}
	
	
	if((status->go==0)&&(hasCoord(status->R,*pos))){
		status->R[pos->x][pos->y] = 0;
		status->cntR--;
		status->found = 1;
	}
	if((status->go==0)&&(hasCoord(status->B,*pos))){
		status->found = 1;
	}
	if((status->go==0)&&!(hasCoord(status->B,*pos))){
		status->B[pos->x][pos->y] = 1;
		status->cntB++;
	}
	
	if(status->found==1&&!(hasCoord(status->S,*pos))){
		status->S[pos->x][pos->y] = 1;
		status->cntS++;
		status->found = 0;
	}
	
	if(status->found==1&&(hasCoord(status->S,*pos))&&!(hasCoord(status->T,*pos))){
		status->T[pos->x][pos->y] = 1;
		status->cntT++;
		Expand(status,pos);
	}
	
}

void Expand (gameState *status, coord *pos){
	
	coord center = *pos;
	coord u,d,k,r;
	
	//Left
	u.x = center.x-1;
	u.y = center.y;
	//Right
	d.x = center.x+1;
	d.y = center.y;
	//Down
	k.x = center.x;
	k.y = center.y-1;
	//Up
	r.x = center.x;
	r.y = center.y+1;
	
	Remove(status,pos);
	if(status->go)
		Replace(status,&u);
	else
		Replace(status,&d);
	
	Replace(status,&k);
	Replace(status,&r);
}

void Update(gameState *status, coord *pos){
	status->good = 0;
	if(hasCoord(status->S,*pos)==0){
		status->S[pos->x][pos->y]=0;
		status->cntS--;
		status->good = 1;
	}
	if(status->good==0&&hasCoord(status->S,*pos)&&!(hasCoord(status->T,*pos))){
		status->T[pos->x][pos->y] = 1;
		status->cntT++;
		Expand(status,pos);
	}
}

void NextPlayerMove (coord *pos){
	
}

// Evaluates the board when the game is over and prints the winner
int GameOver(gameState *status) {
    int resultCode = -1; // Default safe value instead of -888
    
    // Check if the game is actually over using our checkGame function
    int over = !checkGame(*status);

    if (over) {
        // Check who has the higher piece count when the game ends
        if (status->cntR > status->cntB) {
            printf("R wins\n");
            resultCode = 1;
        } else if (status->cntR < status->cntB) {
            printf("B wins\n");
            resultCode = 2;
        } else if (status->cntR == status->cntB) {
            printf("draw\n");
            resultCode = 0;
        }
    }
    
    // Returns 1 for R, 2 for B, 0 for draw, or -1 if the game isn't actually over yet
    return resultCode; 
}

int checkGame(gameState status){
    int isPlaying = 1; // default to 1 (game is still going)
  
    // Game ends if F=3, val>=20, or if someone has 0 pieces after the start phase.
    if(status.cntF == 3 || status.val >= 20 || 
       (status.start == 0 && ((status.cntR > 0 && status.cntB == 0) || (status.cntB > 0 && status.cntR == 0)))){
        
        isPlaying = 0; // conditions met, game is over
    }
    
    return isPlaying; 
}

//initializes all array elements to 0 (NULL)
void ZeroArray (int array[][4]){
	int i,j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++)
			array[i][j] = 0;
	}
}

//checks if the coord is valid returns 1 if yes, 0 otherwise;
int checkValid(coord pos){
	if((pos.x>0&&pos.x<4)&&(pos.y>0&&pos.y<4))
		return 1;
	return 0;	
}

//Checks if given coordinate is in the given array (set)
//Returns 1 if TRUE, 0 if FALSE
int hasCoord(int array[][4],coord pos){
	if(array[pos.x][pos.y]==1)
		return 1;
	return 0;
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

//Set cardinalities of each array to Zero
status.cntR = 0;
status.cntB = 0;
status.cntS = 0;
status.cntT = 0;


coord pos; //This is NOT part of the list of variables from MP specs


while (checkGame(status)){ //Game Loop Control
	
}
  	  

	return 0;
}
