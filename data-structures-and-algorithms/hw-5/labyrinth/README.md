## Labyrinth ##
Find the length of the shortest path through a labyrinth. The labyrinth is described as matrix contained of 0s and 1s. The 0 represents a cell in the labyrinth and the 1 - an obstacle.
Starting from the cell in the upper left corner (the one with coordinates (0,0)) you must find the shortest path to cell (x_i, y_i), as the permitted turns are moving upwards, downwards, leftwards and rightwards with just one position.
If a given cell is an obstacle you do not have the right to enter into it.
### Input format ###
On the first line of the standard input you are give three numbers: N, M and K.<br/>
M - number of rows in the matrix.<br/>
N - number of columns in the matrix.<br/>
K - number of queries.<br/>
On each of the next M rows are given the elements building up the matrix as 1 stands for an obstacle and 0 for a cell you can be in.<br/>
On each of the next K rows are given couples of numbers representing a cell's coordinates - x_i y_i. That is the cell you must move to.
