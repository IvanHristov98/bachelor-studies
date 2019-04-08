## Shortest path ##
By a given unoriented and weighted graph, a starting vertex s, find the shortest path from s to every other vertex.
The weight of each edge within the graph is non-negative.
### Input format ###
On the first row is give a number t - number of the tests. Each test consists of:<br/>
<i>On the first row are given the numbers n and m - the number of the vertices and the number of edges.
Each of the next m rows contains the numbers u, v and w, corresponding to a unoriented edge (u,v) with a weight of w.
On the last row is given a number s - the starting vertex. The vertices within the graph a labeled with the numbers from 1 to n.</i>
### Output format ###
For each test output dist[0], dist[1], ..., dist[s-1], dist[s+1], ..., dist[n], where dist[i] is the shortest path from s to i.<br/>
If there is no such path then dist[i] = -1.
