# ece650-Waterloo-Traffic-Management-System
To help the local police department minimize their installation of security cameras at traffic intersections - based on Vertex Cover problem

* Languages: Python, C++
* Multithread
* Cmake
* MiniSAT
* Linux
* Multiprocessing


### A1
#### Sample Input
The input comprises lines each of which specifies a command. There are 4 kinds of commands.
* add a street
* change a street
* remove a street
* generate a graph

Here is an example of how your program should work. Visualizing this example using the Cartesian coordinate system may help you understand what’s going on.
```Python
a "Weber Street" (2,-1) (2,2) (5,5) (5,6) (3,8)
a "King Street S" (4,2) (4,8)
a "Davenport Road" (1,4) (5,8)
g
V={
  1: (2,2) 
  2: (4,2) 
  3: (4,4) 
  4: (5,5) 
  5: (1,4) 
  6: (4,7) 
  7: (5,6) 
  8: (5,8) 
  9: (3,8) 
  10: (4,8)
} 

E={
  <1,3>,
  <2,3>,
  <3,4>,
  <3,6>,
  <7,6>,
  <6,5>,
  <9,6>,
  <6,8>,
  <6,10>
}
```

#### Commands
* a is used to add a street. It is specified as: “a "Street Name" (x1, y1) (x2, y2) . . . (xn, yn)”. Each (xi, yi) is a GPS coordinate. We interpret the coordinates as a poly-line segment. That is, we draw a line segment from (xi, yi) to (xi+1, yi+1). You are allowed to assume that each xi and yi is an integer. (Note, however, that the coordinates of an intersection may not be integers.)
* c is used to change the specification of a street. Its format is the same as for a. It is a new specification for a street you’ve specified before.
* r is used to remove a street. It is specified as “r "Street Name"”. 
* g causes the program to output the corresponding graph.

### A2
For this assignment, you need to write a program that takes input till it sees an EOF.
* One of the kinds of input contributes towards the specification of an undirected graph.
* Another kind of input asks you to print out a shortest-path from one vertex to another in the
current graph.

#### Sample Run
Assume that your executable is called a2-ece650. In the following, “$” is the command-prompt.
```Python
$ ./a2-ece650
V 15
E {<2,6>,<2,8>,<2,5>,<6,5>,<5,8>,<6,10>,<10,8>} s 2 10
2-8-10
V5
E {<0,2>,<2,1>,<2,3>,<3,4>,<4,1>}
s40
4-1-2-0
```

#### Input, Output, and Error
Your program should take input from standard input, and output to standard output. Errors can also be output to standard output, but should always start with “Error:” followed by a brief description. Your program should terminate gracefully (and quietly) once it sees EOF. Your program should not generate any extraneous output; for example, do not print out prompt strings such as “please enter input” and things like that.

As the example above indicates, one kind of input is the specification of a set of vertices V, and set of edges E of the undirected graph. The specification of a set of vertices starts with ’V’, followed by a space, followed by a positive integer, all in one single line. If the integer that follows the V is i, then we assume that the vertices are identified by 0, . . . , i − 1.

The specification for a set of edges starts with ’E’. It then has a space, followed by the set of edges in a single line delimited by ’{’ and ’}’. The two vertices of an edge are delimited by ’<’ and ’>’ and separated by a comma. The edges in the set are also separated by a comma. There are no whitespace characters within the { }.

The only other kind of input starts with an ’s’. It asks for a shortest path from the first vertex to the second that is specified after the s. The s is followed by a space, a vertex ID, another space, and a second vertex ID. The lines 2-8-10 and 4-1-2-0 above are outputs of the s commands that immediately precede them. The output comprises vertex IDs separated by -, with no whitespace within. If a path does not exist between the vertices, you should output an error.

The graph is specified by the specification of the set of vertices V followed by the set of edges E, in that order. V and E always occur together. There is no relationship between subsequent graph specifications; when a new V specification starts, all previous information can be forgotten. After the specification of the graph there can be zero or more shortest-path queries s. For each s query, only one shortest path should be output; multiple shortest paths might exist and an arbitrary choice can be made. Please note that there is no self loop in the graph.

We will not test your program for format errors in the input. That is, our input will be perfectly formed. However, we will test your program for other kinds of errors. For example, if we have V 5, we may try to specify an edge <2,10>, for a vertex 10 that does not exist. Similarly, we may ask for a shortest path to a vertex that does not exist. We may also ask for a shortest path when the vertices exist, but a path does not exist between them. Errors should be output in such cases.

#### CMake
As discussed below under “Submission Instructions”, you should create a CMakeLists.txt file to build your project. We will build your project using the following sequence:
```Python
cd PROJECT && mkdir build && cd build && cmake ../ && make
```

### A3
Modify the output format of your Python script from Assignment 1 to match the input format of your C++ program from Assignment 2.
* Modify your C++ program from Assignment 2 to output the input graph on standard output.
* Write a program in C++ to generate random input for your Python script.
* Write a driver program in C++ that uses Inter-Process Communication (IPC) to link the
output of the random input generator to the input of the Python script, and the output of the Python script to the input of the C++ program from Assignment 2.

#### Sample Run
You should name the drivers executable a3-ece650. In the following, we assume that “$” is the shell command-prompt.
```Python
$ cd build
$ cmake ../
$ make install
$ cd ./run/bin
$ ./a3-ece650 -s 5 -n 4 -l 5
V8
E {<0,2>,<0,3>,<0,4>,<1,3>,<4,7>,<5,2>,<5,6>} s24
2-0-4
```

In the above, the first three lines make your executable, and run the driver program with some command-line arguments. Then, the lines “V = ...”, “E = ...”, and “2-0-4” are output. The input the user provided to your program via stdin is “s 2 4”. Please note that the graph can be generated repeatedly and user can interact with the program with “s” command.

#### Input, Output, and Error
Your program should take input from stdin, and output to stdout. Errors should be output to stderr. Errors should always start with “Error:” followed by a brief description. All your processes should terminate gracefully (and quietly) once you see EOF at stdin. Your program should not generate any extraneous output; for example, do not print out prompt strings such as “please enter input” and things like that.

As the example above indicates, there are two kinds of inputs the user provides. One is via the command-line arguments, like -s and -n switches. This is done only once, when your program is started. The other input is the ’s’ command on stdin, which may be issued repeatedly, just as in Assignment 2. For the ’s’ command, your program should output a shortest path.

We will not test your program for format errors in the input. That is, the command-line arguments, whenever specified, will be formatted correctly, and the s input will also be formatted correctly. Of course, we may omit command-line arguments (see below for what to do in such cases), and specify vertex IDs to s that do not exist, or between whom a path does not exist. The latter two cases should cause your program to report an error.

#### Python script
You should edit your Python script from Assignment 1 so that its output format for the specification of the graph matches the input format for your C++ program from Assignment 2. Think of it as though your Python script provides command-line input to the C++ program. The way to do this is to simply map each vertex to an index in [0, n − 1] (where n is the number of vertices), and rename your edges accordingly.

#### Cpp program from Assignment 2
You should edit it to print the graph that it has read before accepting an s command. This is necessary so that you know what graph has been produced and what vertexes are available.

Also, the only output that your Python script should produce to stdout is in response to “g”, for which it outputs the specification of the graph (i.e., V and E). Error output should go to stderr.

#### Random input generator
Your random input generator rgen should generate random inputs of street specifications for your Python script. It takes four command-line arguments. All are optional.
* -s k — where k is an integer ≥ 2. The number of streets should be a random integer in [2, k]. If this option is not specified, you should use a default of k = 10; that is, the number of streets should be a random integer in [2, 10].
* -n k — where k is an integer ≥ 1. The number of line-segments in each street should be a random integer in [1, k]. Default: k = 5.
* -l k — where k is an integer ≥ 5. Your process should wait a random number w seconds, where w is in [5, k] before generating the next (random) input. Default: k = 5.
* -c k — where k is an integer ≥ 1. Your process should generate (x, y) coordinates such that every x and y value is in the range [−k, k]. For example, if k = 15, all of your coordinate values should be integers between −15 and 15. Default: k = 20.

Your program should generate a specification of streets in the format that your Python script expects (see Assignment 1). You can name the streets whatever you want. You should ensure that your input does not have errors. For example, if you generate a line-segment that partially overlaps with a line-segment (across all streets) generated earlier, you should regenerate that line-segment. Similarly, you should not have any zero-length line segments. Also, segments from same street cannot intersect and overlap.

Also, note that your random generator could go into an infinite loop looking for a valid specifica- tion. You should disallow this from happening by limiting the number of tries. That is, if your ran- dom generator fails to generate a valid specification for a continuous A number of attempts, it should exit() with an error message reported on stderr. A reasonable A to adopt may be 25. Whatever A you adopt, your error message should mention it. That is, your error message should be something like, “Error: failed to generate valid input for 25 simultaneous attempts”.

Before a new specification to your Python script, your generator should issue “r” commands to your Python script to remove all previous streets and replace them with the new specification of streets.

* issue enough r commands to clear any existing street database
* issue a commands to add new streets satisfying the specification
* issue a g command
* wait for specified number of seconds and repeat.

You must use /dev/urandom as the source of your random data. Also, see under “Submission Instructions” how your executable for the random generator must be named.

#### Driver
Your driver program has the overall control. You have at least three programs that run concurrently:
* the random generator
* your Python script from Assignment 1
* your program from Assignment 2 that takes as input a graph-specification and computes shortest paths

Your driver program should fork() two processes and exec() two of those programs, and then exec() the third (so it turns into a process that corresponds to the third program). It should set up all IPC appropriately beforehand.

It should send normal output to stdout, error output to stderr, and take input from stdin. As we mention above, the only input it takes are “s” commands, that ask for a shortest path between vertices.

#### CMake
As discussed below under “Submission Instructions”, you should use a CMakeLists.txt file to build your project. We will build your project using the following sequence:
```Python
cd PROJECT && mkdir build && cd build && cmake ../ && make instal
```
Note that we are using make install instead of make. The install target instructs make to copy all of the binaries (both Python and C++) into directory ./run/bin. This ensures that all the executable files (including the Python program) are in the same directory.

### A4
For this assignment, you are to augment your code from Assignment 2 to solve the minimal Vertex Cover problem for the input graph. Your approach is based on a polynomial time reduction to CNF- SAT, and use of a SAT solver. The following are the steps you should take for this assignment.

#### SAT Solver
We will be using MiniSat SAT solver available at https://github.com/agurfinkel/minisat MiniSat provides a CMake build system. You can compile it using the usual sequence:
```Python
cd PROJECT && mkdir build && cd build && cmake ../ && make
```

The build process creates an executable minisat and a library libminisat.a. You will need link against the library in your assignment.

#### Incorporate SAT
Create a polynomial reduction of the decision version of VERTEX COVER to CNF-SAT. We have discussed the reduction in class. It is also available under the name a4 encoding.pdf on LEARN. You are allowed to use your own reduction provided it is sound and polynomial-time. Implement the reduction and use minisat as a library to solve the minimum VERTEX COVER problem for the graphs that are input to your program (as in Assignment 2).

As soon as you get an input graph via the ’V’ and ’E’ specification you should compute a minimum-sized Vertex Cover, and immediately output it. The output should just be a sequence of vertices in increasing order separated by one space each. You can use qsort(3) or std::sort for sorting.

Assuming that your executable is called a4-ece650, the following is a sample run of your program:
```Python
$ ./a4-ece650
V5
E {<0,4>,<4,1>,<0,3>,<3,4>,<3,2>,<1,3>} 
3 4
```
The lines starting with V and E are the inputs to your program, and the last line is the output. Note that the minimum-sized vertex cover is not necessarily unique. You need to output just one of them.

#### The reduction

Given a pair (G,k) where G = (V,E), denote |V| = n. Assume that the vertices are named 1,...,n. Construct F as follows.
(1) The reduction uses n × k atomic propositions, denoted x<sub>i,j</sub>, where i ∈ [1, n] and j ∈ [1, k]. A vertex cover of size k is a list of k vertices. An atomic proposition x<sub>i,j</sub> is true if and only if the vertex i of V is the j<sup>th</sup> vertex in that list.
(2) The reduction consists of the following clauses
* At least one vertex is the ith vertex in the vertex cover:
∀ i ∈ [1,k], a clause (x<sub>1,i</sub> ∨ x<sub>2,i</sub> ∨ ··· ∨ x<sub>n,i</sub>)
* No one vertex can appear twice in a vertex cover.
∀ m ∈ [1, n], ∀ p, q ∈ [1, k] with p < q, a clause (¬x<sub>m,p</sub> ∨ ¬x<sub>m,q</sub>). In other words, it is not the case that vertex m appears both in positions p and q of the vertex cover.

* No more than one vertex appears in the mth position of the vertex cover.
∀ m ∈ [1,k], ∀ p,q ∈ [1,n] with p < q, a clause (¬x<sub>p,m</sub> ∨ ¬x<sub>q,m</sub>)

* Every edge is incident to at least one vertex in the vertex cover.
∀ ⟨i,j⟩ ∈ E, a clause (x<sub>i,1</sub> ∨ x<sub>i,2</sub> ∨ ··· ∨ x<sub>i,k</sub> ∨ x<sub>j,1</sub> ∨ x<sub>j,2</sub> ∨ ··· ∨ x<sub>j,k</sub>)

### Project
You should augment your code from Assignment 4 in the following ways.
* Make it multithreaded. You should have at least 4 threads: one for I/O, and one each for the different approaches to solve the minimum vertex cover problem.
* Implement the following two additional ways to solve MIN-VERTEX-COVER, in addition to the REDUCTION-TO-CNF-SAT approach you had in Assignment 4. (We will call your approach from Assignment 4, CNF-SAT-VC.)
(1). Pick a vertex of highest degree (most incident edges). Add it to your vertex cover and throw away all edges incident on that vertex. Repeat till no edges remain. We will call this algorithm APPROX-VC-1.
(2). Pick an edge ⟨u,v⟩, and add both u and v to your vertex cover. Throw away all edges attached to u and v. Repeat till no edges remain. We will call this algorithm APPROX- VC-2.

#### Inputs
As input, use the output of /home/agurfink/ece650/graphGen/graphGen on ecelinux. That program generates graphs with the same number of edges for a particular number of vertices, but not necessarily the same edges.

#### Output
Given a graph as input, your program should output the vertex cover computed by each approach in sorted order. That is, give the following input:
```Python
V5
E {<2,1>,<2,0>,<2,3>,<1,4>,<4,3>}
```

The output from your program should be:
```Python
CNF-SAT-VC: 2,4
APPROX-VC-1: 2,4
APPROX-VC-2: 0,2,3,4
```
#### Analysis
You should analyze how efficient each approach is, for various inputs. An input is characterized by the number of vertices. “Efficient” is characterized in one of two ways:
* running time
* approximation ratio

We characterize the approximation ratio as the ratio of the size of the computed vertex cover to the size of an optimal (minimum-sized) vertex cover.

For measuring the running time, use pthread getcpuclockid(). For an example of how it is used, see http://www.kernel.org/doc/man-pages/online/pages/man3/pthread_getcpuclockid.3.html.

For measuring the approximation ratio, compare it to the output of CNF-SAT-VC, which is guaranteed to be optimal.
Your objective is to measure, for various values of |V| (number of vertices), for the graphs generated by graphGen, the running time and approximation ratio. You should do this by generating graphs for |V | ∈ [5, 20] using that program, in increments of 5. That is, graphs with 5, 10, 15, 20 vertices.

You should generate at least 10 graphs for each value for |V |, compute the time and approxi- mation ratio for each such graph. You should measure the running time for at least 10 runs of each such graph. Then, you should compute the mean (average) and standard deviation across those 100 runs for each value of |V |. For the approximation ratio, if there is any random component (e.g., which edges you choose, for APPROX-VC-2), then you should measure that multiple times as well for each graph.
