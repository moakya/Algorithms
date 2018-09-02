I wrote this code in C++, to compile and execute it you can use the command 

./run.sh "*.txt"

where "*.txt" is the file name without ""

Assumptions:
For this problem I assumed that the input will be added by the user at
compilation, in the terminal. 

I create 2 strings to hold the pattern that we are matching for, and the
other to hold the text, that we are matching against. 
The way I have setup the Finite Automata, I setup a table of states, 
to match against all the possible ASCII characters (256), I ensure that
the table is sized correctly.
After that I use a for loop, and call the function Fin_Auto_Match, that
will setup the rest of the table.
In that function if a match is found I return the state value to be added 
into the table in the correct index, otherwise I restart the for loop 
inside the Fin_Auto_Match function. 
After the table has been completely setup, I use a for loop to find all 
the matches, from within the Finite Automata table. As soon as the state
counter reaches the length of the pattern we know that the final state has
been reached and that we have found a match, then I add the index into 
an integer vector, by specifically pushing the index the first character
match was found at, by subtracting the length of the pattern from the index
i, and then subtracting 1 otherwise the match would've been offset.
Then I flip the pattern string, create a new table for this reversed string,
and then repeat the process, finally adding the matches for the reverse into
the same vector we added the forward matches into. 
Then I use insertion sort to make sure that the matches were in order in 
the vector, finally I call another function, that adds all the data in the
vector in the correct order, into the output file.