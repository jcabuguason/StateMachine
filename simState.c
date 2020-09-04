#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct State{
	char key;	
	struct State* next;
	struct State* previous;
}state;

// Method declaration
void printStates();
void nextState(state *current);
void previousState(state * current);
void link2States( state * prevState,state * nextState);
void link3States( state *prevState,state *currentState,state *nextState);
state* findLetter( state *arr[] ,char letter);
state* findState(state*arr [] ,char letter);
void change( state * currentState,char num,state *changeState);
state* createState( char c );
void findGarbage();
void findNonGarbageStates(state * currentState);
int compareStateKeys(state * currentState);
void refresh();
void deleteGarbage();


// instance variable declartion
static int num = 0;
static int garbageCount = 0;
state* sArray[8];
char nonGarbage[] = {'0','0','0','0','0','0','0','0'};
char letters [] = {'A','B','C','D','E','F','G','H'};
char lettersPM [] = {'A','B','C','D','E','F','G','H'};
state* garbageStates[8];

// main method
int main(){
	

	state* traversalState;
	state* stateToDelete;
	int i = 0,j;
	char input,input1,input2;
	char check[2];
	sArray [0] = createState('A');
	sArray [1] = createState('B');
	sArray [2] = createState('C');
	sArray [3] = createState('D');
	sArray [4] = createState('E');
	sArray [5] = createState('F');
	sArray [6] = createState('G');
	sArray [7] = createState('H');

	link3States(sArray[0],sArray[5],sArray[7]);
	link3States(sArray[1],sArray[6],sArray[2]);
	link3States(sArray[2],sArray[3],sArray[1]);
	link3States(sArray[3],sArray[0],sArray[3]);
	link3States(sArray[4],sArray[1],sArray[5]);
	link3States(sArray[5],sArray[2],sArray[7]);
	link3States(sArray[6],sArray[4],sArray[3]);
	link3States(sArray[7],sArray[4],sArray[6]);


	traversalState = sArray[2];
	printf("%c\n", traversalState -> key);

	//user input
	scanf(" %c",&input);

	// user interface 
	while(input != '-'){


		switch(input){

			case '0':
				previousState(traversalState);
				traversalState = traversalState -> previous;
				break;
			case '1':
				nextState(traversalState);
				traversalState = traversalState -> next;
				break;
			case 'c':
				scanf(" %c",&input1);
				scanf(" %c",&input2);
				change(traversalState, input1, findLetter(sArray,input2));
				break;
			case 'p':
				printStates(sArray);
				break;
			case 'g':

				//refresh
				refresh();
				findNonGarbageStates(traversalState);
				findGarbage();
				if(garbageCount > 0){
					printf("\nGarbage:\n");
					for(i = 0 ; i< garbageCount; i++)
						printf("%c\n", garbageStates[i]->key);
				}
				else 
					printf("No garbage\n");
				break;

			case 'd':
				input1 = fgetc(stdin);
				input2 = fgetc(stdin);
				if(input1 == '\n'){
					findGarbage();
					deleteGarbage();
				}
				else{
					stateToDelete = findState(sArray,input2);
					stateToDelete -> key = 'N';
				}

				break;
		}
		scanf(" %c",&input);
	}
}

//printStates method : prints the values of a linked list --> change name for later
void printStates(){

	int i;
	for(i = 0; i<8; i++ ){
		// checks if the state has not been deleted
		if( (sArray[i] -> key) != 'N')
			printf("%c %c %c \n", sArray[i]->key, sArray[i]->previous->key, sArray[i]->next->key);
	}
	
}

// nextState method : displays the next state 
void nextState(state * current){

	if(current != NULL && (current -> next != NULL) ){
		printf("%c \n",current -> next ->key );
	}
	else 
		printf("next state is null or current state cannot be found");
}

// previousState method: displays previous state
void previousState(state *current){

	if(current != NULL && current -> previous != NULL){
		printf("%c \n",current -> previous -> key);
	}
	else
		printf("previous state is null or current state cannot be found.\n");
}


// link states by triples, used in double linked lists 
void link3States( state *currentState,state *prevState,state *nextState){
	
	// link current state previous to previous state
	currentState -> previous = prevState;

	// link current state next to next state
	currentState -> next = nextState;
}



// change method : changes the previous or next state to the inputted state 
void change( state *currentState, char num, state *changeState){

	if(num == '0')
		currentState -> previous = changeState;
	if(num == '1')
		currentState -> next = changeState;
}
	
// finds the state associated with the letter
state* findLetter(state*arr [] ,char letter){
	int i;

	for(i = 0; i < 8 ; i++ ){

		if( arr[i] -> key == letter ){
			return arr[i];
		}
	}

	printf("Error: No such state\n");
	return NULL;
}

// find state associated with letter, this is only used for the delete method
state* findState(state*arr [] ,char letter){
	int i;

	for(i = 0; i < 8 ; i++ ){

		if( arr[i] -> key == letter ){
			return arr[i];
		}
	}

	printf("Not deleted.\n");
	return NULL;
}

// creates a state using memory allocation
state* createState(char c){

	state* current = (state*)malloc(sizeof(state));

	if(current == NULL){
		printf("Memory not allocated.\n");
		return 0;
	}

	current -> key = c;
	return current;
}

// finds non garbage states recursively and puts them in a char array of state -> keys
void findNonGarbageStates(state* currentState){

	int i;

	if( compareStateKeys(currentState) == 0  ) {

		nonGarbage[num] = currentState -> key;
		// printf("\nnonGarbage[%d]: %c" ,num,currentState -> key); //checking delete later
		num++;

		if(currentState -> next -> key != currentState -> key)
			findNonGarbageStates( currentState -> next);

		if(currentState -> previous -> key!= currentState -> key )
			findNonGarbageStates( currentState -> previous);
	}
}

// compare states to the nongarbage array elements
int compareStateKeys(state* currentState ){
	int i,flag;

	for(i = 0 ; i < 8; i++){
		if(currentState -> key == nonGarbage[i]){
			return 1;
		}
	}
	return 0;
}

// refreshes static variables and instance variables 
void refresh(){
	int i;
	num = 0;
	char temp [] = {'A','B','C','D','E','F','G','H'};

	for(i = 0; i < 8; i++){
		nonGarbage[i] = '0';
		letters[i] = temp[i];
	}
}

// find actualy garbage through algorithim
void findGarbage(){
	int i,j,key;
	garbageCount = 0;

	for(i = 0; i< 8; i++)
			for( j = 0; j < 8; j++)
				if(nonGarbage[i] == letters[j]){
					letters[j] = '0';
					break;
				}

	for(i = 0; i< 8; i++)
		for(j = 0 ; j < 8;j++)
			if(sArray[i] -> key == letters[j]){
				garbageStates[garbageCount] = sArray[i];
				garbageCount++;
			}
}

// delete the garbage through an algoritihim
void deleteGarbage(){

	int i,j;

	if(garbageCount == 0)
		printf("No States Deleted\n");
	else{
		printf("\nDeleted: ");
		for(i = 0; i < 8;i++)
			for(j = 0; j < garbageCount; j++)
				if(garbageStates[j] -> key == sArray[i]-> key){
					printf("\n%c ", sArray[i] -> key );
					sArray[i] -> key = 'N';
					break;
				}
		printf("\n");
	}
}




