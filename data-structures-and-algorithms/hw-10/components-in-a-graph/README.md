## Components in a graph ##
There are 2N values to represent nodes in a graph. They are divided into two sets G and B. Each set has exactly N values. 
Set G is represented by {G_1, G_2, ...,G_N}.
G can contain any value between 1 to N (inclusive). Set B is represented by {B_1, B_2, ..., B_N}.
B can contain any value between N + 1 to 2N (inclusive).
Same value can be chosen any number of times.<br/>
Here (G_1, B_1), (G_2, B_2), ..., (G_N, B_N) represents the edges of the graph.
Your task is to print the number of vertices in the smallest and the largest connected components of the graph.<br/>
<b>Note</b> that single nodes should not be considered in the answer.
### Input format ###
First line contains an integer N.<br/>
Each of the next N lines contain two space-separated integers, i-th line contains G_i and B_i.
### Output format ###
Print two space separated integers, the number of vertices in the smallest and the largest components.
