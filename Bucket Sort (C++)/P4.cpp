#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
using namespace std;

void go(ifstream &, ofstream &);	//to start the process
void getGrid(ifstream &, vector<string> &);	//to get names from the file
void bucketSort(vector<string> &, bool &);	//to put elements into buckets
void insertion(vector<string> &, bool &);	//to implement the insertion sort
void write(vector<string> &, ofstream &);	//write the sorted names to a file

int main(int argc, const char * argv[]) {
	ifstream infile;
	ofstream outfile;
	
	infile.open(argv[1]);	//open input file
	outfile.open("output.txt");	//open output file

	if (infile.is_open())
		go(infile, outfile);	//start process

	else{   //File not found!
		cout << "Error opening file! Terminating......." << endl;
		exit (4041);
	}	//end else
	
	infile.close();	//close the input file
	outfile.close();	//close the output file
	return 0;
}	//end main

/***********************************************************************
Pre: Takes in an input file and an output file
Description: Creates a vector and a bool value then starts the process 
	by calling multiple functions and then finally calls write to
	change the output file
Post: -
***********************************************************************/
void go(ifstream &infile, ofstream &outfile) {
	vector<string> strVec;	//create the main vector
	getGrid(infile, strVec);	//get values into the 1D vector

	bool flag = true;	//flag for recursion
	bucketSort(strVec, flag);	//call bucketSort with the vector and flag

	write(strVec, outfile);	//call write to write to output file
}	//end go

/***********************************************************************
Pre: Takes in an input file and a vector
Description: Takes elements out of the file and then adds it to the 
	vector.
Post: -
***********************************************************************/
void getGrid(ifstream &infile, vector<string> &strVec) {
	string str;	//create a string var
	while (infile >> str)	//get text from file into the string
		strVec.push_back(str);	//add the strings to the vector
}	//end getGrid

/***********************************************************************
Pre: Takes in an a vector and a bool 
Description: This function checks the flag; if the flag is true then 
	it creates 26 buckets 1 for each character in the alphabet, and 
	adds elements into it, and then calls insertion. Otherwise if flag 
	is false, it takes the size of the vector that was passed in and 
	divides it by 10 to get a value and create that many number of 
	buckets and then proceed to add elements from the vector to them so
	they contain only 10 elements or less, and then calls insertion on 
	it to sort them.
Post: -
***********************************************************************/
void bucketSort(vector<string> &strVec, bool &flag) {
	
	if (flag == true){
		vector<vector<string> > grid;		//create a 2D vector
		grid.resize(26);	//create 26 buckets
		
		for (int i = 0; i < strVec.size(); i++) {
			string str = strVec[i];	//add the ith element into the string
			int index = str[0] - 'A'; //get the index in which to add the string
			grid[index].push_back(str);	//add the item to the new 2D vector
		}	//end for
		
		for (int i = 0; i < grid.size(); i++)	//pass in each row of the 2D vector
			insertion(grid[i], flag);	//sort each row

		strVec.clear();	//clear the original vector's contents

		for (int i = 0; i < grid.size(); i++)
			for (int j = 0; j < grid[i].size(); j++)
				strVec.push_back(grid[i][j]); //add the sorted names to the original vector
	}	//end if - for recursion

	else {
		

		vector<vector<string> > grid2;		//create a 2D vector
		grid2.resize(26);	//create 26 buckets

		for (int i = 0; i < strVec.size(); i++){
			string str = strVec[i];
			int index = str[1] - 'a';	//use second char to index
			grid2[index].push_back(str);
		}	//end for

		for (int i = 0; i < grid2.size(); i++)	//pass in each row of the 2D vector
			insertion(grid2[i], flag);	//sort each row 
	}	//end else
}	//end sort

/***********************************************************************
Pre: Takes in an a vector and a bool
Description: This function checks the size of the vector being passed in
	if its greater than 10 bucketSort is called again to create more 
	buckets and add elements into those. Otherwise it uses insertion sort 
	to sort the buckets.
Post: -
***********************************************************************/
void insertion(vector<string> &grid, bool &flag) {
	if (grid.size() > 10){	//check if size of vector > 10
		flag = false;	
		bucketSort(grid, flag);	//call bucket sort recursively
	} //end if

	for (int i = 0; i < grid.size(); i++) {
		string key = grid[i];	//get the key
		for (int index = i + 1; index < grid.size(); index++)
			if (grid[index] < grid[i]) {	//compare the key and the next element
				key = grid[index];	//reassign key
				grid[index] = grid[i];	//swap the element
				grid[i] = key;	//swap the other element
			}	//end if
	}	//end for
}	//end insertion sort

/***********************************************************************
Pre: Takes in an a vector and an output file
Description: This function adds elements from the vector into the 
	output file.
Post: -
***********************************************************************/
void write(vector<string> &strVec, ofstream &outfile) {
	for (int i = 0; i < strVec.size(); i++) {
			outfile << strVec[i] << " ";	//add element into the file
	}	//end outer for
}	//end write