## Leaderboard attack ##
Write down an AVL tree that has the options to add and remove items.
### Input format ###
On the first row of the standard input you are given a number N representing the number of the operations. It is followed by N rows each having the format - string argument where string is the operation to do and argument is the number to operate with.<br/>
The commands are as follows - add, contains, remove, print.<br/>
<b>Note</b> out that print doesn't take an argument.<br/>
### Output format ###
When given an add X operation if the number X already exists in the AVL tree print "X already added\n".<br/>
When given remove X if X is not added into the AVL tree then print "X not found to remove".<br/>
When given contains X print "yes" or "no" depending on whether X is present into the tree.<br/>
When given print print the contents of the tree in a pre-order traversal format.
