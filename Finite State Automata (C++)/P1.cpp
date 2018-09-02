#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
using namespace std;

const int MAX_CHARS = 256;	//all available ASCII chars

void go(ifstream &, ofstream &);	//to start the process
vector<int> find (string &, string &);	//main function to find matches
int Fin_Auto_Match (string &, int &, int &);	//to setup the grid for FA
void sort (vector<int> &); //insertion sort to ensure the final results are in order

int main(int argc, const char * argv[]) {
	ifstream infile;	
	ofstream outfile;

	infile.open(argv[1]);	//open the file
	outfile.open("output.txt");	//open output file

	if (infile.is_open())
		go(infile, outfile);	//start process

	else {   //File not found!
		cout << "Error opening file! Terminating......." << endl;
		exit(4041);
	}	//end else

	infile.close();	//close the file
	outfile.close();	//close the output file
	return 0;
}	//end main

/***********************************************************************
Pre: Takes in an input file and an output file
Description: gets the pattern and the text to be matched in, and then 
	creates a vector to hold all the matches, and calls find to add the
	elements into the vector, then finally puts the data from the vector
	into the output file.
Post: -
***********************************************************************/
void go(ifstream &infile, ofstream &outfile) {
	
	string pat;	//to hold the pattern
	infile >> pat;	//get the patern
	
	string txt;	//to hold the text 
	infile >> txt;	//get text

	vector<int> fin = find (pat, txt);	//find the matches and store in fin

	//write the output to the file
	for (int i = 0; i < fin.size(); i++){
		outfile << fin[i];
		if (i != fin.size()-1)
			outfile << " ";	//make sure space isnt added after the last index
	}	//end for
}	//end go

/***********************************************************************
Pre: Takes in 2 strings 1 for the pattern and the other for the text
Description: creates the grid for finite automata and then uses it match
	for the pattern forwards and then backwards, adds the matches i.e.
	the indices where the matches were found.
Post: returns the int vector containing the list of matches
***********************************************************************/
vector<int> find (string &pat, string &txt){
	vector <int> fin;	//to hold the values of match locations
	int patL = pat.length();	//length of pattern
	int txtL = txt.length();	//length of text
	
	/*******************************************************************
								Finding forwards matches
	*******************************************************************/
	vector<vector<int> > transGrid;	//to hold the finite automata grid

	transGrid.resize(patL + 1);	//increase the row size for states

	//resizing each column to accomodate more vals
	for (int i = 0; i < transGrid.size(); i++)
		transGrid[i].resize(MAX_CHARS);

	//COMPUTE-TRANS
	for (int state = 0; state <= pat.length(); state++)
		for (int i = 0; i < MAX_CHARS; i++)
			transGrid[state][i] = Fin_Auto_Match (pat, state, i);

	//use the grid to find all the forward matches
	for (int i = 0, state = 0; i < txtL; i++){
		state = transGrid[state][txt[i]];
		if (state == patL)
			fin.push_back (i - patL + 1);
	}	//end for

	/*******************************************************************
								Finding backwards matches
	*******************************************************************/
	string revPat = "";	//create empty string to contain the new pattern
	
	//add all the elements into the new string backwards
	for (int i = patL - 1; i >= 0; i--)
		revPat += pat[i];
	//length of the reversed pattern will still be the same so reusing var

	//COMPUTE-TRANS
	for (int state = 0; state <= patL; state++)
		for (int i = 0; i < MAX_CHARS; i++)
			transGrid[state][i] = Fin_Auto_Match (revPat, state, i);

	//use the grid to find all the backwards matches
	for (int i = 0, state = 0; i < txtL; i++){
		state = transGrid[state][txt[i]];
		if (state == patL)
			fin.push_back (i - patL + 1);
	}	//end for
	
	/*the vector fin should now contain all matches we found for both
	forwards and backwards*/

	sort (fin);	//sort the vector to ensure the elements are in order

	return fin;	//return the vector containing the matche indices

}	//end find

/***********************************************************************
Pre: Takes in a string and int called state and an int named val 
	containing an index
Description: goes through the string to find at what state a match 
	occurs to ensure that the correct state is added to the grid for 
	string matching.
Post: returns an int that contains the value of the state
***********************************************************************/
int Fin_Auto_Match (string &pat, int &state, int &val){
	//to check whether the next character is identical to the one in the pattern
	//if so retrun the new value of state
	if (state < pat.length() && val == pat[state])
		return (state + 1);
	
	//going through using the max value, to find the prefix that is also a suffix
	for (int nState = state, i = 0; nState > 0; nState--)
		if (pat[nState - 1] == val){
			for (; i < nState - 1; i++)
				if (pat[i] != pat[state + i - nState + 1])
					break;	//restart for loop
			if (i == nState - 1)  
				return nState;	//return the state
		}	//end outer if
}	//end computeTrans

/***********************************************************************
Pre: Takes in an int vector
Description: performs insertion sort on the vector to ensure that the 
	data is in ascending order
Post: -
***********************************************************************/
void sort (vector<int> &fin){
	for (int i = 1, j, val; i < fin.size(); i++){
		j = i - 1;
		val = fin[i];

		while (j >= 0 && fin[j] > val){
			fin[j+1] = fin[j];
			j--;
		}	//end while
		fin[j+1] = val;
	}	//end for
}	//end sort