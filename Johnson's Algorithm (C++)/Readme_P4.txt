I wrote this code in C++, to compile and execute it you can use the command 

./run.sh "*.txt"

where "*.txt" is the file name without ""

Assumptions:
For this problem I assumed that the input will be added by the user at
compilation, in the terminal. 

I used a struct and called it Node, to represent a vertex in the graph. 
It contains, an int for the name of the node, another int to hold the
distance value. A node * value that will point to the parent of the 
current node, and then finally a list of adjacent nodes, that will be
a list of addresses to the other nodes.
I get the input file from the user and create an output file. 
I make the adjacency matrix from the input and then using that matrix, 
I initialize the names of the nodes, and then initialize the adjacency 
list.

To perform Johnsons Algorithm I keep my original graph intact, and 
create a new graph as a copy of the first, I add the Super Vertex to
it and create a new Super Matrix from the original matrix, for use
in Johnson's algoriothm, after which I alter the Adjacenct list for
all the nodes in the new graph to the correct values, so the original
graph ins't changed at all. Then I call Bellman Ford algorithm with
these new objects, and if I get true, use the funtion reweight to change 
d values in the matrix. Then call Dijkstra on all the nodes in the original 
graphm using the altered matrix, adding the resulting values into a new 
matrix, then using the Super Vertex Graph and the new matrix created after
Dijkstra, I unweight the d values, and alter the matrix, and then call 
writeFile to output the data into a file.  