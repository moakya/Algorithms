I wrote this code in C++, to compile and execute it you can use the command 

./run.sh "*.txt"

where "*.txt" is the file name without ""

Assumptions:
For this problem I assumed that the input will be added by the user at
compilation, in the terminal. 

I created a struct Node, to represent a vertex in the graph, it contains a 
string to hold the name of the Node, a list of pointers to all of its adjacent 
nodes, an int value to hold a color value, which is declared as enum, and 
finally 2 more ints one for dicovery time of the node, and the other for the 
finishing time. 
I get the input files, and create an output file, that will contain the result
of the program. I proceed to get the names for the nodes, from every line in 
the input up to the ':', after which I pass the rest of the input into an 
adjacency list, for each node specifically. 
After that I call Depth First Search on the graph, which is a vector of Node 
objects, after the depth first search process is complete, the finish time
is changed. Finally I proceed to sort the graph, in ascending order for the 
finishing times. From there I add the data to the outfile in order.