#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <math.h>
using namespace std;

const int BIG_BOI = 2000000;

struct Node {
    int name,       //the name of the node
        d = BIG_BOI;    //assign all d values to infinity
    Node * pi = NULL;         //the parent of the node 
    vector <Node *> adj;    //this will be the adjacency list 
};

void go(ifstream &, ofstream &);    //to start the process

//function to resize the vector
void resizeGrid(vector <vector <int> > &, int &);

//function to add all the values to the grid
void makeGrid(vector <vector <int> > &, vector<int> &);

//set the adjacency list
void setAdj (vector <Node> &, vector< vector<int> > &);

//perform Johnsons algorithm
void JohnsonsAlg (vector <Node> &, vector< vector<int> > &, ofstream &);

//initialize Super vertex
Node initializeSuper (vector<Node> &);

//create a Bellman Ford matrix
vector< vector<int> > createBFMatrix(vector< vector<int> > &);

//perfom Bellman Ford algorithm
bool bellmanFord (vector <Node> &, vector< vector<int> > &, int &);

//perform relax
void relax (Node & , Node *, vector< vector<int> > &);

//reweight edges
void reweight (vector< vector<int> > &, vector<Node> &);

//perform Dijkstra
void Dijkstra(vector<Node> &, vector <vector<int> > &, int &);

//makes sure values are up to date
void fixGraph (vector<Node> &, vector<Node> &); 

//returns min value and then removes it from the graph
Node * extractMin (vector<Node> &, vector<Node> &);

Node minimum (vector<Node> &);  //returns the minimum node

void buildMinHeap (vector<Node> &); //builds the min heap

void heapify (vector<Node> &, int &); 

//unweight edges
void unweight (vector< vector<int> > &, vector<Node> &);

//output for correct input
void writeFile (ofstream &outfile, vector< vector<int> > &);

//file for negative cycle
void invalidOut (ofstream &outfile);

int main (int argc, const char * argv[]) {
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
}	//end main

/***********************************************************************
Pre: Takes in an input file and an output file
Description: gets the input from the file and adds it into the graph,
    then sets the adjacent nodes list, then performs Johnsons's algorithm
Post: -
***********************************************************************/
void go(ifstream &infile, ofstream &outfile) {
    int temp;   //for the rest of the input

    vector<int> vec;    //to hold initial input

   	while (infile >> temp)
        vec.push_back(temp);    //get the rest of the input

    double nSize = sqrt(vec.size()); //get the size for the 2d vector
    int vecSize = static_cast <int> (nSize);    //make sure that the size is an int

    vector <vector <int> > matrix;  //to hold the adjacency matrix
    resizeGrid(matrix, vecSize);    //makes sure size is right

    makeGrid(matrix, vec);  //make the adjacency matrix

    //add all the names to the graph
    vector<Node> graph; 
    graph.resize(matrix.size());
    for (int i = 0; i < graph.size(); i++)
        graph[i].name = i;
    
    setAdj(graph, matrix);  //set adjacent nodes

    JohnsonsAlg (graph, matrix, outfile);
}   //end go

/***********************************************************************
Pre: Takes in a 2d vector of ints, and an int
Description: resizes the vector, to ensure all our data can fit
Post: -
***********************************************************************/
void resizeGrid(vector <vector <int> > &grid, int &mySize) {
    grid.resize(mySize);    //resize rows
    for (int i = 0; i < grid.size(); i++)
        grid[i].resize(mySize); //resize each column
}   //end resizeGrid

/***********************************************************************
Pre: Takes in a 2d vector of ints, and a 1d vector of ints
Description: parses the data in the 1d vector, in the correct order and 
    adds it to the 2d vector, to make the final adjacency matrix we will 
    be using
Post: -
***********************************************************************/
void makeGrid(vector <vector <int> > &grid, vector<int> &vec) {
    int val = grid.size();

    for (int i = 0; i < val; i++)
        for (int j = 0; j < val; j++) {
            int temp = vec.front(); //to hold a temp value
            vec.erase(vec.begin() + 0); //delete that value in the vector
            grid[i][j] = temp;  //assign the  value in the matrix
        }   //end inner for
}   //end makeGrid

/***********************************************************************
Pre: Takes in a vector of Node objects, and a 2d vector of ints
Description: goes through the 2d vector of ints i.e. the matrix and
    assigns values in the Node vector to the correct adjacency list in
    the correct locations.
Post: -
***********************************************************************/
void setAdj (vector <Node> &graph, vector< vector<int> > &matrix){
    for (int i = 0; i < graph.size(); i++){
        vector<int> temp;   //temp vector
        for (int j = 0; j < matrix.size(); j++)
            //add the ith row of the matrix for processing 
            temp.push_back(matrix[i][j]);
        
        for (int j = 0; j < temp.size(); j++)
            if (temp[j] != BIG_BOI && j != i)
                //adds the address of the correct node into the adjacency list
                graph[i].adj.push_back(&graph[j]);        
    }   //end for
}   //end setAdj

/***********************************************************************
Pre: Takes in a vector of Node objects, and a 2d vector of ints,
    and an ofstream object (output file)
Description: performs Johnsons Algorithm on the graph and the matrix
    in multiple steps. Then finally calls one of the 2 output 
    functions depending.
Post: -
***********************************************************************/
void JohnsonsAlg (vector<Node> &graph, vector< vector<int> > &matrix, ofstream &outfile){
	vector<Node> nGraph = graph;   //create a copy of the graph
	Node super = initializeSuper (nGraph); //create a super node from the new graph
	nGraph.push_back(super);   //add the super node to the graph
	//create a new adjacency matrix to help with new graph
    vector< vector<int> > nMatrix = createBFMatrix (matrix);
	
	for (int i = 0; i < nGraph.size(); i++)
		nGraph[i].adj.clear();    //clear original adjcacency pointers

	setAdj(nGraph, nMatrix);   //reassign pointers within new graph

	//perform Bellman Ford and return a bool value
    bool flag = bellmanFord (nGraph, nMatrix, nGraph[nGraph.size() - 1].name);
	
	if (flag){
		reweight (matrix, nGraph); //reweight edges
		vector< vector<int> > grid;   //new matrix for output
		grid.resize(matrix.size());   
		for (int i = 0; i < graph.size(); i++){
			Dijkstra(graph, matrix, i);  //perform Dijkstra on each Node
			for (int j = 0; j < grid.size(); j++)
				grid[i].push_back(graph[j].d);  //add outputs
		}	//end outer for
		unweight(grid, nGraph);	//unweight edges
		writeFile(outfile, grid); //write correct output file
	}	//end flag
	
	else invalidOut(outfile);  //negative cycle file
}	//end JohnsonsAlg

/***********************************************************************
Pre: Takes in a vector of Nodes
Description: using the graph, initializes a Super vertex
Post: return the super node
***********************************************************************/
Node initializeSuper (vector<Node> &graph){
    Node super;    //create node
    super.name = graph.size(); //assign a name
    super.d = 0;   //default d value
    super.pi = NULL;   //no parent

    for (int i = 0; i < graph.size(); i++)
        super.adj.push_back(&graph[i]);      //add addresses to adj list

    return super; 
}   //end initializeSuper

/***********************************************************************
Pre: Takes in a 2D vector of ints
Description: creates a copy of the original matrix then adds values for
    the Super Vertex
Post: returns the new matrix 
***********************************************************************/
vector< vector<int> > createBFMatrix(vector< vector<int> > &matrix){
    vector< vector<int> > nMatrix; //create the new matrix
    nMatrix = matrix;  //copy

    vector<int> temp;  //temp vector 
    for (int i = 0; i < matrix.size(); i++)
        temp.push_back(0);    //add 0's for edge weights

    nMatrix.push_back(temp);   //push all the values into new matrix

    for (int i = 0; i < nMatrix.size() - 1; i++)
        nMatrix[i].push_back(2000000);    //add infinity to show no edge

    nMatrix[nMatrix.size() - 1].push_back(0);  //add last value d value to itself

    return nMatrix;    //return the new matrix
}   //end createBFMatrix

/***********************************************************************
Pre: Takes in a vector of Nodes, 2D vector of ints and an int
Description: performs Bellman Ford Algorithm on the graph using the matrix
    and calling relax
Post: returns bool value depending on wheter a negative cycle was found
***********************************************************************/
bool bellmanFord (vector <Node> &graph, vector< vector<int> > &matrix, int &source){
    for (int i = 0; i < graph.size() - 1; i++)  //iteration counter
        for (int j = graph.size() - 1; j >= 0; j--) //on all nodes
            for (int k = 0; k < graph[j].adj.size(); k++)  //on all adjacents 
                relax (graph[j], graph[j].adj[k], matrix);    //relax

    //check for negative cycles
    for (int i = 0; i < graph.size() - 1; i++)  
        for (int j = 0; j < graph[i].adj.size(); j++)
            if (graph[i].adj[j]->d > graph[i].d + matrix[graph[i].name][graph[i].adj[j]->name])
                return false; //negative cycle

    return true;    //non negative cycle
}   //end bellmanFord

/***********************************************************************
Pre: Takes in a Node object pointer, a Node object and a 2d int vector
Description: performs relax using the Node pointer and the node
    by utilizing the values in the adjacency matrix
Post: -
***********************************************************************/
void relax (Node &u, Node *v, vector< vector<int> > &matrix){
    if (v->d > u.d + matrix[u.name][v->name]){
        v->d = u.d + matrix[u.name][v->name];   //change d value
        v->pi = &u;   //change parent
    }   //end if
}   //end relax

/***********************************************************************
Pre: Takes in a 2D vector of ints, and a vector of Nodes
Description: reweights the edges for Johnsons algorithm
Post: -
***********************************************************************/
void reweight (vector< vector<int> > &matrix, vector<Node> &graph){
    for (int i = 0; i < matrix.size(); i++)
        for (int j = 0; j < matrix[i].size(); j++)
            if (matrix[i][j] != BIG_BOI) //only reweight if not infinity
                matrix[i][j] = matrix[i][j] + graph[i].d - graph[j].d; //reweight
}   //end reweight

/***********************************************************************
Pre: Takes in a vector of Node objects, and a 2d vector of ints, and 
    an int value
Description: performs Dijkstra's algorithm on the vector of Node objects
    by utilizing the adjacency matrix, and the int source
Post: -
***********************************************************************/
void Dijkstra(vector<Node> &graph, vector <vector<int> > &matrix, int &source){
    for (int i = 0; i < graph.size(); i++)
        graph[i].d = BIG_BOI;

    graph[source].d = 0;    //initialize the source d value to 0

    vector<Node> nGraph;    
    nGraph = graph;    //make a copy of the graph to be processed

    while (nGraph.size() != 0){
        Node* obj = extractMin(nGraph, graph);  //address of min d value holder
        for (int i = 0; i < obj->adj.size(); i++){
            relax (*obj, obj->adj[i], matrix);  //perform relax
            fixGraph(nGraph, graph);    //make sure graph is up-to-date
        }   //end for
    }   //end while
}   //end Dijkstra

/***********************************************************************
Pre: Takes in 2 vectors of Node objects
Description: makes sure that the vector we are using in Dijkstra's
    algorithm has all the values in the correct place and that 
    all the values are correct.
Post: -
***********************************************************************/
void fixGraph (vector<Node> &nGraph, vector<Node> &graph){
    for (int i = 0; i < nGraph.size(); i++)
        for (int j = 0; j < graph.size(); j++)
            if (nGraph[i].name == graph[j].name)   
                nGraph[i] = graph[j];   //update the value if names are the same
}   //end fixGraph

/***********************************************************************
Pre: Takes in 2 vector of Nodes objects 
Description: Calls on maximum to get the max value, then remove the 
    extracted value from the vector
Post: Return the min value pointer separately so it can be used.
***********************************************************************/
Node* extractMin (vector<Node> &nGraph, vector<Node> &graph){ 
    Node myObj = minimum(nGraph); //obj that contains min

    Node * obj; //node pointer
    for (int i = 0; i < graph.size(); i++)
        if (graph[i].name == myObj.name)
            obj = &graph[i];    //assigns the address of the node in graph

    nGraph.erase(nGraph.begin());

    return obj;    //return the min value address
}   //end extractMax

/***********************************************************************
Pre: Takes in a vector of nodes objects
Description: calls buildMinHeap 
Post: return the first object in the vector
***********************************************************************/
Node minimum (vector<Node> &graph){
    buildMinHeap(graph);  //get Max Heap
    return graph[0];  //return the first object
}   //end maximum

/***********************************************************************
Pre: Takes in a vector of Nodes objects 
Description: Calls heapify multiple times to get a min heap
Post: -
***********************************************************************/
void buildMinHeap (vector<Node> &graph){
    for (int i = (graph.size() / 2) - 1; i >= 0; i--)
        heapify (graph, i);
}   //end buildMaxHeap

/***********************************************************************
Pre: Takes in a vector of Node objects and an int 
Description: Using the index, creates a heap 
Post: -
***********************************************************************/
void heapify (vector<Node> &graph, int &index){
    int smallest = index;    //assume index is the smallest
    int left = (2 * index), //left child
        right = (2 * index) + 1;    //right child

    //if left child < parent then reassign smallest to left
    if (left < graph.size() && graph[left].d < graph[index].d)
        smallest = left;
    
    //if right child < parent then reassign smallest to right
    if (right < graph.size() && graph[right].d < graph[index].d)
        smallest = right;

    //if laregest != index swap elements and recursively call heapify again
    if (smallest != index){
        Node temp = graph[index]; //temp = index obj
        graph[index] = graph[smallest]; //index obj = smallest obj
        graph[smallest] = temp;    //smallest obj = temp
        heapify(graph, smallest);
    }   //end if
}   //end maxHeapify

/***********************************************************************
Pre: Takes in a 2D vector of ints, and a vector of Nodes
Description: unweights the edges for Johnsons algorithm
Post: -
***********************************************************************/
void unweight (vector< vector<int> > &matrix, vector<Node> &graph){
    for (int i = 0; i < matrix.size(); i++)
        for (int j = 0; j < matrix[i].size(); j++)
            if (matrix[i][j] != BIG_BOI) //only unweight if not infinity
                matrix[i][j] = matrix[i][j] + graph[i].d - graph[j].d;  //unweight
}   //end unweight

/***********************************************************************
Pre: Takes in a 2D vector of int, and an ofstream object (output file)
Description: adds the output to the output file
Post: -
***********************************************************************/
void writeFile (ofstream &outfile, vector< vector<int> > &grid){
    for (int i = 0; i < grid.size(); i++){
        for (int j = 0; j < grid[i].size(); j++){
            outfile << grid[i][j];   //add the value
            if (j != grid[i].size()-1)
                outfile << " "; //add space
        }   //end inner for
        if (i != grid.size() - 1)
            outfile << endl;    //add new line
    }  //end outer for

}   //end writeFile

/***********************************************************************
Pre: Takes in an ofstream object (output file)
Description: Adds line saying negative Cycle
Post: -
***********************************************************************/
void invalidOut (ofstream &outfile){
	outfile << "Negative cycle";
}   //end invalidOut\