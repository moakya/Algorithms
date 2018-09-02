#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <math.h>
using namespace std;

const int BIG_BOI = 2000000;    //value to hold infinity

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

//begin Dijkstras Algorithm
void Dijkstra(vector<Node> &, vector <vector<int> > &, int &);

//perform relax
void relax (Node* , Node &, vector< vector<int> > &);

//makes sure values are up to date
void fixGraph (vector<Node> &, vector<Node> &); 

//returns min value and then removes it from the graph
Node * extractMin (vector<Node> &, vector<Node> &);

Node minimum (vector<Node> &);  //returns the minimum node

void buildMinHeap (vector<Node> &); //builds the min heap

void heapify (vector<Node> &, int &); 

//setup the output for the file
vector<int> getEnd (vector<Node> &, int &, int &);

//write the output to the file
void writeFile (ofstream &, vector<Node> &, vector<int> &, int &);

int main(int argc, const char * argv[]) {
    ifstream infile;
    ofstream outfile;

    infile.open(argv[1]);   //open the file
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
    then sets the adjacent nodes list, then performs Dijkstra's algorithm
    and writes the output to the file.
Post: -
***********************************************************************/
void go(ifstream &infile, ofstream &outfile) {
    int source,
        destination,
        temp;   //for the rest of the input

    infile >> source;
    infile >> destination;
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

    Dijkstra(graph, matrix, source); //perform Dijkstra's algorithm

    //get the final output 
    vector<int> fin = getEnd(graph, source, destination);

    //write the output to the file
    writeFile (outfile, graph, fin, destination);
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
Pre: Takes in a vector of Node objects, and a 2d vector of ints, and 
    an int value
Description: performs Dijkstra's algorithm on the vector of Node objects
    by utilizing the adjacency matrix, and the int source
Post: -
***********************************************************************/
void Dijkstra(vector<Node> &graph, vector <vector<int> > &matrix, int &source){
    graph[source].d = 0;    //initialize the source d value to 0

    vector<Node> nGraph;    
    nGraph = graph;    //make a copy of the graph to be processed

    while (nGraph.size() != 0){
        Node* obj = extractMin(nGraph, graph);  //address of min d value holder
        for (int i = 0; i < obj->adj.size(); i++){
            relax (obj, *obj->adj[i], matrix);  //perform relax
            fixGraph(nGraph, graph);    //make sure graph is up-to-date
        }   //end for
    }   //end while
}   //end Dijkstra

/***********************************************************************
Pre: Takes in a Node object pointer, a Node object and a 2d int vector
Description: performs relax using the Node pointer and the node
    by utilizing the values in the adjacency matrix
Post: -
***********************************************************************/
void relax (Node *u, Node &v, vector< vector<int> > &matrix){
    if (v.d > u->d + matrix[u->name][v.name]){
        v.d = u->d + matrix[u->name][v.name];   //change d value
        v.pi = u;   //change parent
    }   //end if
}   //end relax

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
Pre: Takes in a vector of Node objects, and 2 ints
Description: go by the destination and go up to the source, adding the 
    names of the nodes into an int vector
Post: return the int vector for the output file
***********************************************************************/
vector<int> getEnd (vector<Node> &graph, int &source, int &destination){
    vector<int> fin;   //to hold all the final values
    fin.push_back(destination); //add the destination node

    int i = destination;   //set the counter
    while (graph[i].pi != NULL){
        int temp = graph[i].pi->name;   //get the name of the parent
        fin.push_back(temp);   //add it to the vector
        i = temp;   //update the counter
    }   //end while

    return fin; //return the vector
}   //getEnd

/***********************************************************************
Pre: Takes in an ofstream object, a vector of Node objects, an int vector
    and an int
Description: goes throgh the int vector and adds the elements in reverse
    order into the file to ensure that we start at the source and 
    diplay up to the destination. 
Post: -
***********************************************************************/
void writeFile (ofstream &outfile, vector<Node> &graph, vector<int> &fin, int &destination){
    outfile << graph[destination].d;    //add the shortest path
    outfile << ": ";    //add the separator

    for (int i = fin.size() - 1; i >= 0; i--){
        outfile << fin[i];  //add the path
        if (i != 0)
            outfile << " ";
    }   //end for
}   //end writeFile