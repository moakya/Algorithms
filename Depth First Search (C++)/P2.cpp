#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sstream>
using namespace std;

struct Node {
    string name;    //the name of the node
    vector<Node *> adj; //list of pointers to adjacent nodes
    vector <string> adjacent;   //to hold list of adjacent nodes
    int col,    //color of the node
        disc,   //discovery time
        fin;  //finish time
};

enum Color {WHITE, GREY, BLACK};

void go(ifstream &, ofstream &);    //to start the process

void updateName (vector <string> &, vector <Node> &);   //to get names for nodes

void updateAdj ( vector<string> &, vector<Node> &); //get the adjacent nodes

void DFS (vector <Node> &); //start DFS

void DFS_Visit (vector<Node> &, int &, int &); //auxilary function for DFS

int findIndex (vector<Node> &, string &); //returns index of node we are searching

void sort (vector<Node> &); //insertion sort to ensure the final results are in order

void writeOutput (vector<Node> &, ofstream &);    //writes the output to the file

int main (int argc, const char * argv[]){
    ifstream infile;    
    ofstream outfile;

    infile.open(argv[1]);   //get the file
    outfile.open("output.txt"); //open output file

    if (infile.is_open())
        go(infile, outfile);    //start process

    else {   //File not found!
        cout << "Error opening file! Terminating......." << endl;
        exit(4041);
    }   //end else

    infile.close(); //close the file
    outfile.close();    //close the output file
    return 0;
}   //end main

/***********************************************************************
Pre: Takes in an input file and an output file
Description: gets the input from the file and adds it into the graph,
    using updateName and updateAdj, then performs DFS, and writes to 
    an output file.
Post: -
***********************************************************************/
void go(ifstream &infile, ofstream &outfile){
    vector <string> inVec;  //to contain the entire input
    string temp;    
    while (getline(infile, temp))
        inVec.push_back (temp);

    vector<Node> graph; //the graph 

    updateName(inVec, graph);   //get the names into the graph
    updateAdj(inVec, graph);    //get all the adjacent nodes

    DFS(graph);     //perform Depth First Search
  	
  	sort(graph);	//sort the nodes in ascending order by finish times

    writeOutput(graph, outfile);	//write the output to the file
}   //end go

/***********************************************************************
Pre: Takes in vector of strings and a vector of nodes
Description: goes through the string vector and extracts all the names
    from it and changes the names of all the nodes in the graph. 
Post: -
***********************************************************************/
void updateName (vector <string> &inVec, vector <Node> &graph){
    vector <string> nameVec;    //create a vector to hold all names

    for (int i = 0, j; i < inVec.size(); i++){
            j = 0;
            string str = inVec[i];  //temporarily store the entire string
            for (; str[j] != ':'; j++){} //empty for loop
            string nstr = str.substr (0, j);    //get the name
            nameVec.push_back(nstr);    //add the name to the vector
    }   //end for

    graph.resize(nameVec.size());   //make sure we have enough nodes

    //add all the names for the nodes
    for (int i = 0; i < graph.size(); i++)
        graph[i].name = nameVec[i];
}   //end getName

/***********************************************************************
Pre: Takes in vector of strings and a vector of nodes
Description: goes through the string vector and extracts all the values 
    for the adjacenct nodes, and puts them into a string vector,
    then processes that vector and changes the node's adjacent pointers 
    list to point to the correct nodes.  
Post: -
***********************************************************************/
void updateAdj (vector <string> &inVec, vector<Node> &graph){
    vector<string> adjList;

    for (int i = 0, j; i < inVec.size(); i++){
            j = 0;
            string str = inVec[i];  //temporarily store the entire string
            for (; str[j] != ':'; j++){} //empty for loop
            string nstr = str.substr (j+1);    //get the list of adjacents
            adjList.push_back(nstr);    //add the adjacents to the vector
    }   //end for

    for (int i = 0; i < graph.size(); i++){
        stringstream sstr;      //stringstream object
        sstr << adjList[i];     //put the string into the object
        string nstr;            //create a temporary string
        while (sstr >> nstr)
            //add the strings into the string vector in the node object
            graph[i].adjacent.push_back(nstr);
    }   //end for

    //process the list inside the node to point to the right nodes
    for (int i = 0; i < graph.size(); i++)
        for (int j = 0; j < graph[i].adjacent.size(); j++){
            string str = graph[i].adjacent[j];  //temp string
                for (int k = 0; k < graph.size(); k++)
                    if (graph[k].name == str)   //check if its a match
                        graph[i].adj.push_back(&graph[k]); //assign the pointer
        }   //end inner for
}   //end getAdj

/***********************************************************************
Pre: Takes in a vector of nodes
Description: Performs Depth First Search on the vector of nodes  
Post: -
***********************************************************************/
void DFS (vector <Node> &graph){
    //assign all the node colors to white by default
    for (int i = 0; i < graph.size(); i++)
        graph[i].col = WHITE;
	
	int time = 0;  //initialize time to 0

    //begin the rest of DFS
	for (int i = 0; i < graph.size(); i++)
		if (graph[i].col == WHITE)
			DFS_Visit(graph, i, time);
}   //end DFS

/***********************************************************************
Pre: Takes in a vector of nodes, an int for the index of the node
    being processed, and the time value as an int
Description: Performs the rest of the DFS, recursively.
Post: -
***********************************************************************/
void DFS_Visit (vector<Node> &graph, int &index, int &time){
	time++;    //increment time
	graph[index].disc = time;  //update dicovery time 
	graph[index].col = GREY;   //change the color to grey

	for (int i = 0; i < graph[index].adj.size(); i++){
        Node * ptr = graph[index].adj[i];   //alias for the struct 
		if (ptr -> col == WHITE){ //if node color is white
			//index for the next node we will process
            int nDex = findIndex(graph, ptr -> name);
			DFS_Visit(graph, nDex, time);    //call DFS_Visit recursively
		}	//end if
    }   //end for

	graph[index].col = BLACK;  //change color of node to black
	time++;    //increment time
	graph[index].fin = time;   //update finish time of the node
}	//end DFS-Visit

/***********************************************************************
Pre: Takes in a vector of nodes, and a string
Description: goes through the vector of nodes to find one that matches 
    the string we passed in, if a match is found we return the index.
Post: returns an int
***********************************************************************/
int findIndex (vector<Node> &graph, string &str){
	for (int i = 0; i < graph.size(); i++)
		if (graph[i].name == str) //match is found
			return i;
}	//end findIndex

/***********************************************************************
Pre: Takes in a Node object vector
Description: performs insertion sort on the vector to ensure that the 
    data is in ascending order
Post: -
***********************************************************************/
void sort (vector<Node> &graph){
    Node val;
    for (int i = 1, j; i < graph.size(); i++){
        j = i - 1;
        val = graph[i];

        while (j >= 0 && graph[j].fin > val.fin){
            graph[j+1] = graph[j];
            j--;
        }   //end while
        graph[j+1] = val;
    }   //end for
}   //end sort

/***********************************************************************
Pre: Takes in a vector of Node, and an output file
Description: goes through the vector of and adds the name values of the
	Node to the output file, ensuring that no space is added at the end
	of the file.
Post: -
***********************************************************************/
void writeOutput (vector<Node> &graph, ofstream &outfile){
    for (int i = 0; i < graph.size(); i++){
        outfile << graph[i].name;
        if (i != graph.size()-1)
            outfile << " "; //make sure space isnt added after the last index
    }   //end for
}   //end writeOutput