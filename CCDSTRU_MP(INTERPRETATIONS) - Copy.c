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
    int go; //manages turn taking
    int start; //Is game still in start phase?
    int found; 
    int val; //Number of turns completed
    
}gameState;

//Function prototypes
int checkValid(coord pos);
int hasCoord(int array[][4],coord pos);
void Expand (gameState *status, coord *pos);
int checkGame(gameState status);
int numElement(int set[4][4]);

/*
FUNCTION: Remove

	PURPOSE: Removes a coordinate from the player's set, and updates the cardinality of each set
	RETURN: None
	@param: status, holds the game variables such as the sets, whos turn it is, etc.
	@param: pos, the coordinate being removed from a set
	PRECONDITION:All parameters contains VALID values

*/

void Remove (gameState *status, coord *pos){
	if(!checkValid(*pos)){
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

/*
FUNCTION: Replace

	PURPOSE: 
	RETURN: None
	@param: status, holds the game variables such as the sets, whos turn it is, etc.
	@param: pos, the coordinate being removed from a set
	PRECONDITION:All parameters contains VALID values

*/


void Replace(gameState *status, coord *pos){
	
	if(checkValid(*pos)) 
		return;
		
	status->found = 0; //initialization of found
	
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
		status->S[pos->x][pos->y]=1;
		status->cntS++;
		status->good = 1;
	}
	if(status->good==0&&hasCoord(status->S,*pos)&&!(hasCoord(status->T,*pos))){
		status->T[pos->x][pos->y] = 1;
		status->cntT++;
		Expand(status,pos);
	}
}

void NextPlayerMove(gameState *state, coord *pos) {
    
    int over_state = checkGame(*state);

    if (over_state && state->start && state->go) {
        state->R[pos->x][pos->y] = 1;
        state->cntR++;
        state->S[pos->x][pos->y] = 1;
        state->cntS++;
        state->good = 1;
    } else if (over_state && state->start && !state->go) {
        state->B[pos->x][pos->y] = 1;
        state->cntB++;
        state->S[pos->x][pos->y] = 1;
        state->cntS++;
        state->good = 1;
    } else if (over_state && !state->start) {
        if ((state->go && state->R[pos->x][pos->y]) || (!state->go && state->B[pos->x][pos->y])) {
            Update(state, pos);
            state->good = 1;
        }
    }
    
    if (state->start && numElement(state->R) == 1 && numElement(state->B) == 1) {
        state->start = 0;
    }
    
    if (over_state && state->good) {
        state->good = !state->good; 
        state->go = !state->go;     
        state->val++;
    }
}

//void NextPlayerMove (gameState* status, coord *pos){
//	
//
//	if(checkGame(*status)){ // check muna if status is a go.
//    // If starting phase.
//	    if (status->start) {
//	        if (status->go) { // R's turn
//	            status->R[pos->x][pos->y] = 1; // Adding this coords to the the set R. 
//	            status->cntR++; // Increase ung count ng piece ng R.
//	            status->S[pos->x][pos->y] = 1; // Add htis coords to the set S which are yung selected coords.
//	            status->cntS++;
//	            status->good = 1;
//	        } else { // B's turn
//	            status->B[pos->x][pos->y] = 1;
//	            status->cntB++;
//	            status->S[pos->x][pos->y] = 1;
//	            status->cntS++;
//	            status->good = 1;
//	        }
//	    } else if(checkGame(*status)&&!status->start){
//	        // If not starting phase anymore.
//	        if ((status->go && hasCoord(status->R, *pos)) || // If R player's move, and R has these coords
//	            (!status->go && hasCoord(status->B, *pos))) { // If B player's move, and B has these coords.
//	            	Update(status, pos); 
//	           		status->good = 1;
//	      	  }
//	   		}
//	}
//
//    if (status->start && status->cntR == 1 && status->cntB == 1) { // Once both have played EXACTLY 1 piece on the board, then start phase over.
//        status->start = 0;
//	}
//	
//	if(checkGame(*status)&&status->good){
//		status->good = 0;
//		status->go = !(status->go);
//		status->val++;
//	}
//}

// Evaluates the board when the game is over and prints the winner
int GameOver(gameState *status) {
    int resultCode = -1; // Default safe value instead of -888
    
    // Check if the game is actually over using our checkGame function
    int over = !checkGame(*status);

    if (over) {
        // Check who has the higher piece count when the game ends
        if (status->cntR > status->cntB) {
            resultCode = 1;
        } else if (status->cntR < status->cntB) {
            resultCode = 2;
        } else if (status->cntR == status->cntB) {
            resultCode = 0;
        }
    }
    
    // Returns 1 for R, 2 for B, 0 for draw, or -1 if the game isn't actually over yet
    return resultCode; 
}

int numElement(int set[4][4]) {
    int count = 0;
    int j, i;
    for (i = 1; i <= 3; i++) {
        for (j = 1; j <= 3; j++) {
            if (set[i][j]) count++;
        }
    }
    return count;
}


int checkGame(gameState status){
    int isPlaying = 1; // default to 1 (game is still going)
  	
  	
  	status.cntF = 9 - (numElement(status.R) + numElement(status.B));
  	
    // Game ends if F=3, val>=20, or if someone has 0 pieces after the start phase.
    if(status.cntF == 3 ||
    
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
		return 0;
	return 1;	
}

//Checks if given coordinate is in the given array (set)
//Returns 1 if TRUE, 0 if FALSE
int hasCoord(int array[][4],coord pos){
	if(array[pos.x][pos.y]==1)
		return 1;
	return 0;
}

//Prints the grid
void displayGrid(gameState status){
	int i,j;
	printf("   1   2   3  ");
	printf("\n -------------\n");
	for(i=1;i<4;i++){
		printf("%d|",i);
		for(j=1;j<4;j++){
			if(status.B[j][i]==1){
				printf(" B |");
			} else if (status.R[j][i]==1){
				printf(" R |");
			} else{
				printf(" O |");
			}
		}
		printf("\n -------------\n");
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

//Set cardinalities of each array to Zero
status.cntR = 0;
status.cntB = 0;
status.cntS = 0;
status.cntT = 0;


coord pos; //This is NOT part of the list of variables from MP specs

while (checkGame(status)){ //Game Loop Control
	pos.x = 0;
	pos.y = 0;

	displayGrid(status);
	
	if(status.start==1){
		printf("START PHASE: PLEASE PLACE YOUR INITIAL POINT\n");
	}else{
		printf("CAPTURE PHASE: ONLY SELECT YOUR POINTS\n");
	}
	
	if(status.go==1){ //Tell user who's turn it is
		printf("\nTURN #%d: IT IS RED's TURN\n",status.val+1);
	}else{
		printf("\nTURN #%d: IT IS BLUE's TURN\n",status.val+1);
	}
	
	//Receive User input + input validation
	do{
		
		printf("Please input a coordinate \nX:Y:\n");
		scanf("%d %d",&pos.x, &pos.y);
		printf("You have inputted: (%d,%d)\n\n",pos.x,pos.y);
		
		
		if(checkValid(pos)){ 
			printf("+---------------------------------+\n");
			printf("| INVALID INPUT, PLEASE TRY AGAIN |\n");
			printf("+---------------------------------+\n");
		}
		fflush(stdin);
	}while(checkValid(pos)); //Repeat loop if invalid input
	
	NextPlayerMove(&status,&pos);

printf("\n--------------------------------------------------------\n");
	
}

printf("+-----------------+\n");
printf("|    GAME OVER    |\n");
printf("+-----------------+\n");

displayGrid(status);

switch(GameOver(&status)){
	case 1:
		printf("RED WINS\n");
		break;
	
	case 2:
		printf("BLUE WINS\n");
		break;
	case 0:
		printf("DRAW\n");
		break;
	
}
  	  

	return 0;
}
