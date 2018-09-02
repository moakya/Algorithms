I wrote this code in C++, to compile and execute it you can use the command 

./run.sh "*.txt"

where "*.txt" is the file name without ""


Assumptions:
In this file I assumed that the input will be added by the user at
compilation, in the terminal. 

First of all I created a struct to contain a string to hold the name of the customer 
and an int value to hold the priority.
Then I passed the input and output files to a function that starts the process of the 
heap sort. I used an array representation of the heap sort, by using vectors. 
While getting the input from the file, I ignored the ':' character, and
added the name and the priority.
After that I created a vector that would contain the priorities in 
descending order, I called my extractMax function multiple times, to
get the max values and add them in the correct order to the final vector.
The extractMax function call maximum each time, which calls buildMaxHeap
where I use a for loop to call heapify multiple times to create a maxHeap.
Then after the final vector is in order I call write and the function adds
the data to the output file.

In this file I assumed that the output file created will be called 
output.txt
