I wrote this code in C++, to compile and execute it you can use the command 

./run.sh "*.txt"

where "*.txt" is the file name without ""


Assumptions:
In this file I assumed that the input will be added by the user at
compilation, in the terminal. 

First of all I pass in input and output files into my go function which
starts the entire process. I create a string vector and pass it into the 
getGrid function which add all items from the input file into the vector.
After that I create a flag variable, for recursion in the bucket sort function. 
Then I pass in the vector into the bucket sort function with the flag 
set to true (which represents the first run of the bucket sort function). 
I then create a 2D vector of strings, each row represents an 
character in the english alphabet, and the columns contain the strings 
that begin with that alphabet.
After the items are in the correct buckets (the 26 rows of the vector),
each row of the vector is then passed into the insertion sort function one by one, 
where the size of the row is checked to make sure that the bucket 
doesn't contain more than 10 elements, if this is true; the row is passed 
back and the flag is changed. 
If the vector is sent back into the bucketSort, the function creates 26 more buckets and adds 
the elements into them, indexing using the second char in the name,
 and then calls insertion on them to sort them correctly.
Once all the buckets are sorted, I create a new vector in which I add all
the sorted items, and then pass that final vector into the write function
to add these sorted data items into the output file. 

In this file I assumed that the output file created will be called 
output.txt
