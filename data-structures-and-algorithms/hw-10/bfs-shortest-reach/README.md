## BFS shortest reach ##
Consider an undirected graph where each edge is the same weight. Each of the nodes is labeled consecutively.

You will be given a number of queries. For each query, you will be given a list of edges describing an undirected graph. After you create a representation of the graph, you must determine and report the shortest distance to each of the other nodes from a given starting position using the breadth-first search algorithm (BFS). Distances are to be reported in node number order, ascending. If a node is unreachable, print
for that node. Each of the edges weighs 6 units of distance.
### Input format ###
The first line contains an integer - the number of queries. Each of the following sets of lines has the following format:

The first line contains two space-separated integers and the number of nodes and edges in the graph.
Each line of the subsequent lines contains two space-separated integers and describing an edge connecting node to node.
The last line contains a single integer denoting the index of the starting node.
