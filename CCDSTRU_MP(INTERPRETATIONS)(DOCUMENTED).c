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
	
	// If the coordinate is invalid, exit immediately.
	if(checkValid(*pos)) 
		return;
		
	status->found = 0; //initialization of found
	
	if((status->go==1)&&(hasCoord(status->B,*pos))){ // If R selects a B piece, remove from B.
		status->B[pos->x][pos->y] = 0;
		status->cntB--; // Decrease B count.
		status->found = 1; // A piece has been captured.
	}
	if((status->go==1)&&(hasCoord(status->R,*pos))){ //If R selects its own piece, just mark found.
		status->found = 1;
	}
	if((status->go==1)&&!(hasCoord(status->R,*pos))){ // If R selects an empty spot, place an R piece there.
		status->R[pos->x][pos->y] = 1;
		status->cntR++; // Increase piece count.
	}
	
	// BLUE TURN
	
	if((status->go==0)&&(hasCoord(status->R,*pos))){ // If B selects an R piece, remove from R.
		status->R[pos->x][pos->y] = 0;
		status->cntR--; // Decrease R count.
		status->found = 1; // A pece has been captured.
	}
	if((status->go==0)&&(hasCoord(status->B,*pos))){ // If B selects own piece, just mark found.
		status->found = 1;
	}
	if((status->go==0)&&!(hasCoord(status->B,*pos))){ // If B selects empty spot, place a B piece there.
		status->B[pos->x][pos->y] = 1;
		status->cntB++;
	}
	
	// UPDATE
	
	if(status->found==1&&!(hasCoord(status->S,*pos))){ // If a capture happened and pos is not yet in S, add it.
		status->S[pos->x][pos->y] = 1;
		status->cntS++; // Increase selected count.
		status->found = 0; // Reset
	}
	
	if(status->found==1&&(hasCoord(status->S,*pos))&&!(hasCoord(status->T,*pos))){ // If capture happened, pos is already in S but not yet in T
		status->T[pos->x][pos->y] = 1;
		status->cntT++; // Increase touched count.
		Expand(status,pos); // Trigger expansion.
	}
	
}

/*
FUNCTION: Expand

    PURPOSE: Handles the expansion mechanic when a coordinate is touched. 
             Removes the center piece and spreads influence to its 
             neighboring coordinates (left, right, up, down). 
             Expansion direction depends on the current player's turn.

    RETURN: None

    @param: status, holds the game variables such as the sets, turn state, 
            and counters for each set.
    @param: pos, the coordinate at the center of expansion.

    PRECONDITION: All parameters contain VALID values. Coordinate must 
                  be within the valid board range (1–3 for both x and y).
*/

void Expand (gameState *status, coord *pos){
	
	coord center = *pos; // Copy the current coordinate into a local variable.
	coord u,d,k,r; // Neighbor coordinates.
	
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
	
	Remove(status,pos); // Remove center piece from all sets.
	//Apply replace logic depending on whose turn it is.
	//If R's turn, expand left.
	//If B's turn, expand right
	if(status->go)
		Replace(status,&u);
	else
		Replace(status,&d);
	
	Replace(status,&k);
	Replace(status,&r);
}

/*
FUNCTION: Update

    PURPOSE: Updates the Selected (S) and Touched (T) sets when a player 
             chooses a coordinate. Determines if the move is a new selection 
             or if it triggers expansion through the T set.

    RETURN: None

    @param: status, holds the game variables such as the sets, turn state, 
            and counters for each set.
    @param: pos, the coordinate being processed for update.

    PRECONDITION: All parameters contain VALID values. Coordinate must 
                  be within the valid board range (1–3 for both x and y).
*/

void Update(gameState *status, coord *pos){
	status->good = 0; // Reset
	
	// If coordinate is NOT yet in selected set.
	if(hasCoord(status->S,*pos)==0){
		status->S[pos->x][pos->y]=1; // Add to S
		status->cntS++; // Increase count of S
		status->good = 1; // Valid update occured.
	}
	
	// If coordinate is already in S but not yet in T
	if(status->good==0&&hasCoord(status->S,*pos)&&!(hasCoord(status->T,*pos))){
		status->T[pos->x][pos->y] = 1; // Add to touched set
		status->cntT++; // Increase count of T
		Expand(status,pos); // Trigger expansion.
	}
}

/*
FUNCTION: NextPlayerMove

    PURPOSE: Executes the logic for a player's move depending on the 
             current phase of the game (start phase or capture phase). 
             Handles piece placement, updates sets, and manages turn 
             progression.

    RETURN: None

    @param: status, holds the game variables such as the sets, turn state, 
            counters, and phase indicators.
    @param: pos, the coordinate chosen by the current player.

    PRECONDITION: All parameters contain VALID values. Coordinate must 
                  be within the valid board range (1–3 for both x and y).
*/

void NextPlayerMove (gameState* status, coord *pos){
	
    // Store the state so we don't call the function multiple times
    int isPlaying = checkGame(*status);

	if(isPlaying){ // check muna if status is a go.
        // If starting phase.
	    if (status->start) {
	        if (status->go) { // R's turn
	            status->R[pos->x][pos->y] = 1; // Adding this coords to the the set R. 
	            status->cntR++; // Increase ung count ng piece ng R.
	            status->S[pos->x][pos->y] = 1; // Add htis coords to the set S which are yung selected coords.
	            status->cntS++;
	            status->good = 1;
	        } else { // B's turn
	            status->B[pos->x][pos->y] = 1;
	            status->cntB++;
	            status->S[pos->x][pos->y] = 1;
	            status->cntS++;
	            status->good = 1;
	        }
	    } else {
	        // If not starting phase anymore.
            // (Replaced hasCoord with direct array access)
	        if ((status->go && status->R[pos->x][pos->y]) || // If R player's move, and R has these coords
	            (!status->go && status->B[pos->x][pos->y])) { // If B player's move, and B has these coords.
	            	Update(status, pos); 
	           		status->good = 1;
	      	  }
	   		}
	}

    // Use numElement() instead of status->cntR to prevent desyncs
    if (status->start && numElement(status->R) == 1 && numElement(status->B) == 1) { // Once both have played EXACTLY 1 piece on the board, then start phase over.
        status->start = 0;
	}
	
	if(isPlaying && status->good){
		status->good = 0;
		status->go = !(status->go);
		status->val++;
	}
}

/*
FUNCTION: GameOver

    PURPOSE: Evaluates the board state when the game has ended and 
             determines the winner based on piece counts. 
             Returns a code indicating the result.

    RETURN: Integer result code
            - 1 : Red wins
            - 2 : Blue wins
            - 0 : Draw
            - -1: Game is not yet over

    @param: status, holds the game variables such as the sets, counters, 
            and current state of play.

    PRECONDITION: All parameters contain VALID values. Function should 
                  only be called once the game loop has ended or when 
                  checking for a winner.
*/

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

/*
FUNCTION: numElement

    PURPOSE: Counts the number of elements present in a given set. 
             Iterates through the 3×3 playable grid and returns the 
             cardinality (total count of occupied coordinates).

    RETURN: Integer
            - The number of elements found in the set.

    @param: set, a 4×4 integer array representing a player's set 
            (R, B, S, or T). Only indices 1–3 are valid for both 
            dimensions, corresponding to the playable board area.

    PRECONDITION: The set array must be properly initialized with 
                  values {0,1}, where 1 indicates presence of a 
                  coordinate and 0 indicates absence.
*/

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

/*
FUNCTION: checkGame

**PURPOSE**: Determines the current state of the game to see if play should 
          continue. It calculates the remaining free spaces and checks for 
          specific endgame triggers: if the free spaces reach a certain 
          threshold, or if a player has been eliminated after the initial 
          start phase.

**RETURN**: int (1 if the game is still active, 0 if a game-over condition is met)

**@param**: status, a gameState structure containing the current sets (R, B), 
           player piece counts (cntR, cntB), and the start phase flag.

**PRECONDITION**: The gameState 'status' must be initialized. The numElement 
              function must be defined to correctly count members in sets 
              R and B.
*/

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


/*
FUNCTION: checkValid

**PURPOSE**: Validates whether a given coordinate falls within the 
          allowable 3x3 grid boundaries (1 to 3 for both x and y 
          axes). It acts as a safety gate to prevent out-of-bounds 
          errors during gameplay or expansion.

**RETURN**: int (0 if the coordinate is valid, 1 if it is invalid)

**@param**: pos, a coord structure containing the x and y integer 
           values to be checked.

**PRECONDITION**: The coord structure 'pos' must be properly 
              initialized with integer values.
*/

int checkValid(coord pos){
	if((pos.x>0&&pos.x<4)&&(pos.y>0&&pos.y<4))
		return 0;
	return 1;	
}

/*
FUNCTION: hasCoord

**PURPOSE**: Checks if a specific coordinate on a 2D grid is currently 
          occupied or marked. It looks up the value at the given 
          x and y positions within the provided array to determine 
          presence.

**RETURN**: int (1 if the coordinate is occupied/marked, 0 otherwise)

**@param**: array, a 4x4 2D integer array representing the grid state.
**@param**: pos, a coord structure containing the x and y indices 
           to be checked.

**PRECONDITION**: The coord 'pos' should be validated (e.g., by 
              checkValid) to ensure its x and y values fall within 
               the 0–3 range to prevent memory access errors.
*/

int hasCoord(int array[][4],coord pos){
	if(array[pos.x][pos.y]==1)
		return 1;
	return 0;
}

//Prints the grid, that's it.
void displayGrid(gameState status){
	int i,j;
	printf("\nY-------------\n");
	for(i=1;i<4;i++){
		printf("%d|",i);
		for(j=1;j<4;j++){
			if(status.B[j][i]==1){
		        if(status.S[j][i]==1) printf(" B*|"); // Highlights selected pieces
		        else printf(" B |");
		    } else if (status.R[j][i]==1){
		        if(status.S[j][i]==1) printf(" R*|");
		        else printf(" R |");
		    } else{
		        printf(" O |");
		    }
		}
		printf("\n -------------\n");
	}
	printf("X: 1   2   3  \n\n");
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
