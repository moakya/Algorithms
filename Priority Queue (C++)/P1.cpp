#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
using namespace std;

struct customer{
	string name;
	int priority;
};

void go (ifstream&, ofstream &);  //function to begin process
void insert (vector<customer> &, customer &);  //inserts elements into vector
customer maximum (vector<customer> &);  //returns the max value of the vector
customer extractMax (vector<customer> &);  //returns max value and then removes it from the vector
void heapify (vector<customer> &, int &); 
void buildMaxHeap (vector<customer> &); //builds the max heap
void write (vector<customer> &, ofstream &);  //writes output to the file

int main(int argc, const char * argv[]){
	
	ifstream infile;
	ofstream outfile;
	infile.open(argv[1]);	//open input file
	outfile.open("output.txt"); //open output file

	if (infile.is_open())
		go(infile, outfile);	//start process
	
	else{   //File not found!
		cout << "Error opening file! Terminating......." << endl;
		exit (4041);
	}	//end else
	
	infile.close();	//close the file
	outfile.close(); //clost the output file

	return 0;

}	//end main

/***********************************************************************
Pre: Takes in an input file and an output file
Description: Extracts data from the file passes it to insert, after 
	which extractMax is called repeatedly to get the final vector
	Then it finally writes the output to the file.
Post: -
***********************************************************************/
void go (ifstream& infile, ofstream &outfile){
	
	vector<customer> custVec;	//create the customer vector
	string str;

	while (infile >> str){
		customer nCust;
		nCust.name = str; //change the value of the name
		
		infile.ignore(1, ':'); //skip the :

		int val;
		infile >> val;
		nCust.priority = val;	//add the int priority

		insert (custVec, nCust);	//add the object to the vector
	}	//end while

	vector<customer> fin;	//create final vector
	
	//go through and all max values to the final vector
	for (; custVec.size() != 0;){
		customer temp = extractMax (custVec); //get the Max
		fin.push_back(temp);	//add it to the final vector
	}	//end for
	
	write(fin, outfile);	//output to the file
}	//end go

/***********************************************************************
Pre: Takes in a vector of customer objects and a customer object
Description: Adds the data from the object into the vector
Post: -
***********************************************************************/
void insert	(vector<customer> & custVec, customer& nCust){
		custVec.push_back(nCust);	//add data to vector
}	//end insert

/***********************************************************************
Pre: Takes in a vector of customer objects 
Description: Calls on maximum to get the max value, then remove the 
	extracted value from the vector
Post: Return the max value object separately so it can be used.
***********************************************************************/
customer extractMax (vector<customer> &custVec){
	customer cust = maximum(custVec); //obj that contains max
	vector<customer> temp;	//temp vector to alter original
	
	for (int i = 1; i < custVec.size(); i++)
		temp.push_back(custVec[i]);	//push all values expcept max into temp
	
	custVec = temp;	//alter original vector
	return cust;	//return the max value
}	//end extractMax

/***********************************************************************
Pre: Takes in a vector of customer objects
Description: calls buildMaxHeap 
Post: return the first customer object
***********************************************************************/
customer maximum (vector<customer> &custVec){
	buildMaxHeap(custVec);	//get Max Heap
	return custVec[0];	//return the first object
}	//end maximum

/***********************************************************************
Pre: Takes in a vector of customer objects 
Description: Calls heapify multiple times to get a max heap
Post: -
***********************************************************************/
void buildMaxHeap (vector<customer> &custVec){
	for (int i = (custVec.size() / 2) - 1; i >= 0; i--)
		heapify (custVec, i);
}	//end buildMaxHeap

/***********************************************************************
Pre: Takes in a vector of customer objects and an int 
Description: Using the index, creates a heap 
Post: -
***********************************************************************/
void heapify (vector<customer> &custVec, int &index){
	
	int largest = index;	//assume index is the largest
	int left = (2 * index),	//left child
		right = (2 * index) + 1;	//right child

	//if left child > parent then reassign largest to left
	if (left < custVec.size() && custVec[left].priority > custVec[index].priority)
		largest = left;
	
	//if right child > parent then reassign largest to right
	if (right < custVec.size() && custVec[right].priority > custVec[index].priority)
		largest = right;

	//if laregest != index swap elements and recursively call heapify again
	if (largest != index){
		customer temp = custVec[index];	//temp = index obj
		custVec[index] = custVec[largest]; //index obj = largest obj
		custVec[largest] = temp;	//largest obj = temp
		heapify(custVec, largest);
	}	//end if
}	//end maxHeapify


/***********************************************************************
Pre: Takes in a vector of customer objects and an output file 
Description: goes through the vector and adds the data to the file
Post: -
***********************************************************************/
void write (vector<customer> &custVec, ofstream &outfile){
	for(int i = 0; i < custVec.size(); i++)
		 outfile << custVec[i].name << "  "; //add data to file
}	//end print