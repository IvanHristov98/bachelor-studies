## Railway ##
In the land of Algorithmia as well as <b>roads</b> can be found multiple <b>railways</b> connecting the towns.
Every <b>road</b> is a bidirectional one connecting two towns. The <b>railways</b>s are bidirectional as well but they connect only the captial of Algorithmia with one of the other towns.
Also it should be noted out that each road and each railway have lengths.<br/>
The metropolitans, as always tending to be greedy, want to spare some money by removing a part of the railways, but only at the condition that the removal won't increase the length of the path from the capital to the town.<br/>
Your task is to output the maximum number of railways that could be removed without increasing the length of the path from the capital to any of the towns.
There could be more that one road between any two towns, as well as there could be more than one railway from the capital to any of the towns.
A <b>path</b> from the capital to a town is any sequence of roads and railways starting from the capital and reaching to the town.
The length of that <b>path</b> is the sum of the lengths of the roads and the railways present within the path.
### Input format ###
On the first row are given the numbers n, m and k.<br/>
The towns within Algorithmia are labeled with the numbers from 1 to n, as the capital is labeled with the number 1.<br/>
n - number of towns<br/>
m - number of roads<br/>
k - number of railways<br/>
On each of the next m lines are given the numbers u, v and w, which correspond to a <b>road</b> (u,v) with a weight of w.<br/>
On each of the next k lines are given the numbers u and w, representing a <b>railway</b> from the capital to town u with a weight of w.
### Output format ###
On a single row output the maximum number of railways which could be removed without changing the minimum length from the capital to any of the towns.
