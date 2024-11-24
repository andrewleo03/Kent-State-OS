Andrew Leonard <br>
Kent State University <br>
Operating System Project 2 <br>
<br>
The goal of the assignment was to implement the Banker's Algorithm<br>
in order to avoid deadlock. In addition, the code was made to work on <br> 
a snapshot of any system as long as the input follows the required format <br>
<br>
The format that I implemented in order to read in system information which <br> 
specifies the current number of processes, the number of resources, and the <br> each 
process's current resource allocation, and maximum required resources<br>
is specified below.
<br><br>
*Line 1 contains the names of each resource delimited by a space character
<br>*Line 2 contains the system totals for each resource in the same order they were listed
<br>*Lines 3 and beyond contain information specific to each process grouped by process<br>
<br>Example <br>
Process0 *Name of the Process<br>4 3 2 1 5
*Currently Allocated Resources<br> 
8 9 7 3 9 *Maximum Resources
<br><br>This could be scaled to an arbitrary number of processes. The entirety of the <br> 
implementation of the Banker's Algorithm itself lies inside the isSafe() function. <br> The 
bulk of the rest of the implementation lies inside the "System" class which <br>
keeps everything very organized. There is a debugPrint() member function, that <br> 
when called on a "System" object will print out the snapshot of the resources and <br>
process, to ensure everything was read in correctly.<br><br>
A makefile is included for a convenient build process, there is usage of the C++ <br>
standard library, but it should run with any mainstream implementation of the standard <br>
library.