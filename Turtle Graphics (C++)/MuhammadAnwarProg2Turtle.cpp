#include <iostream>
#include <fstream>
using namespace std;

const int ARRAY_SIZE = 100;

void fileValidation(ifstream &infile);
void initialize(int turtleArray[ARRAY_SIZE][ARRAY_SIZE]);
void movement(ifstream &infile, int turtleArray[ARRAY_SIZE][ARRAY_SIZE]);
void displayArray(int turtleArray[ARRAY_SIZE][ARRAY_SIZE]);

int main() {

	ifstream infile;
	int turtleArray[ARRAY_SIZE][ARRAY_SIZE];

	fileValidation(infile); //open file
	initialize(turtleArray); //set array to zero
	movement(infile, turtleArray); //make changes to array
	displayArray(turtleArray); //display array

	infile.close();

	return 0;
} //end main

/************************************************************************
Function: fileValidation

Description: To open the file turtle.txt and validate it.

Pre: -

Post: File passed on to movement
************************************************************************/

void fileValidation(ifstream &infile) {
	infile.open("turtle.txt");
	while (!infile) {
		cout << "Error opening the file. Terminating...";
		exit(0001);
	} //end while

} //end fileValidation

/************************************************************************
Function: initialize

Description: To initialize all values in the array to 0.

Pre: Array must be declared.

Post: -
************************************************************************/

void initialize(int turtleArray[ARRAY_SIZE][ARRAY_SIZE]) {
	for (int a = 0; a < ARRAY_SIZE; a++)
		for (int b = 0; b < ARRAY_SIZE; b++) {
			turtleArray[a][b] = 0;
		} //end nested for
} //end initialize

/************************************************************************
Function: movement

Description: To read the contents of the file. To make changes to array 
			 accordingly.

Pre: File from fileValidation. Array initialized to 0 from initialize.

Post: -
************************************************************************/

void movement(ifstream &infile, int turtleArray[ARRAY_SIZE][ARRAY_SIZE]) {
	int direction = 2, //1-North 2-East 3-South 4-West
		curX = 0, //current position in the rows
		curY = 0, //current position in the columns
		numForward; //total number of steps moving forward
	char letter; //to contain the command from the file
	bool penPos = 0; //default pen position up (0)

	while (infile.get(letter)) {

		if (letter == 'D')
			penPos = true;

		else if (letter == 'U')
			penPos = false;

		else if (letter == 'R') {
			if (direction == 1) //if North
				direction = 2;	//now East

			else if (direction == 2) //if East
				direction = 3; //now South

			else if (direction == 3) //if South
				direction = 4; //now West

			else if (direction == 4) //if West
				direction = 1; //now North
		} //end else if R

		else if (letter == 'L') {
			if (direction == 1) //if North
				direction = 4; //now West

			else if (direction == 2) //if East
				direction = 1; //now North

			else if (direction == 3) //if South
				direction = 2; //now East

			else if (direction == 4) // if West
				direction = 3; //now South
		} //end else if L

		else if (letter == 'F') {

			infile >> numForward;

			if (direction == 1) {
				for (int i = curX; i > curX - numForward; i--) {
					if (penPos == true)
						turtleArray[i][curY] = 1;
					else
						turtleArray[i][curY];
				} //end for
				curX -= numForward - 1;
			} //end NORTH ~ if

			else if (direction == 2) {
				for (int i = curY; i < curY + numForward; i++) {
					if (penPos == true)
						turtleArray[curX][i] = 1;
					else
						turtleArray[curX][i];
				} //end for
				curY += numForward - 1;
			} //end EAST ~ if 

			else if (direction == 3) {
				for (int i = curX; i < curX + numForward; i++) {
					if (penPos == true)
						turtleArray[i][curY] = 1;
					else
						turtleArray[i][curY];
				} //end for
				curX += numForward - 1;
			} //end SOUTH ~ if

			else if (direction == 4) {
				for (int i = curY; i > curY - numForward; i--) {
					if (penPos == true)
						turtleArray[curX][i] = 1;
					else
						turtleArray[curX][i];
				} //end for
				curY -= numForward - 1;
			} //end WEST ~ if

		} //end else if letter F

	} //end while
} //end movement

/************************************************************************
Function: displayArray

Description: To display the array.

Pre: Declared array.

Post: -
************************************************************************/

void displayArray(int turtleArray[ARRAY_SIZE][ARRAY_SIZE]) {
	for (int c = 0; c < ARRAY_SIZE; c++)
		for (int d = 0; d < ARRAY_SIZE; d++) {
			if (turtleArray[c][d] == 1)
				cout << "*"; //display * instead of every occurance of 1
			else if (turtleArray[c][d] == 0)
				cout << " "; //display white space instead of every occurance of 0
			if (d == 99) 
				cout << endl; //end the line if last column is reached in every row
		} //end nested for
} //end displayArray