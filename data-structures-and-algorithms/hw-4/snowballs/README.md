## Snowballs ##
Winter's coming. A group of people have created a special cloning device that clones a given snowman.
In the beginning they have 0 levs of snowman with 0 balls onto it.
### Input format ###
On the standard input you recieve a number N queries.

p m - cloning of the pth snowman (0 <= p < i, where i is the number of the current query). After cloning you add a ball with weight of m on top of the new clone.
It's guaranteed that you would clone only existing balls.

p 0 - cloning pth snowman and removal of the topmost ball of new clone.
It's guaranteed that you won't  remove a ball from the 0th snowman.
After all of the N queries output the sum of all of the created snowmen.
