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
I used a heap to ensure that I could perform Dijkstra's Algorithm. 
Because in a minHeap, Nodes are destroyed, I create a duplicate, and 
make changes to it, and at the same time keep the values in the original
graph up-to-date, after which I also ensure that the copy of the graph, 
after the min has been extracted is also in the correct order, that is 
where the function fixGraph comes in, and ensures that everything is 
in order. 
Lastly, I use the function getEnd to add the shortest path from the source
to the destination in reverse order, into an integer vector.
Then I pass the final vector into the writeFile function, that 
adds the output to the file in the correct order. 