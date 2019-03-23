## Brand new ##
A friend of yours lives into a strange neighbourhood. The neighbourhood has the form of a rooted tree with numbered nodes from 1 to N and respectively N-1 edges in between them.
Your friends laptop has broken and you must reach to a nearby service in order to fix it.
His house is at the root. The bad thing is that your friend has spent nearly all of his money for the new laptop and he doesn't wish to pass close to a cafe in order not to spend all of his money.
He cannot go to a service if there are more than M consecutive cafes in the path.
Your task is to find the number of services your friend could reach.
### Input format ###
On the first row of the standard input are given two numbers:<br/>
N - number of nodes in the tree<br/>
M - the number of maximum consecutive cafes that won't tempt your friend<br/>
On the second row are N numbers a_i where:<br/>
a_i is 0 or 1, depending on whether there is a cafe in node i.<br/>
They are followed by N-1 edges in the format i j, which are undirected edges into the tree (1≤ i, j≤ N, i ≠ j).<br/>
Tip - It is not mandatory that the i and j have to be already given nodes. For example of row 3 you may be given the edge 8 13.
