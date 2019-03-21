## Path ##
In the magical world of Algorithmia can be found multiple paths. Each path contains of a sequence of rocks - white, green and blue ones.
The rocks are numbered as well. The green and blue rocks have to be replaced by white ones by the following rules:<br/>
If a green rock with the number K is found it is removed and also the previous K rocks are replaced by a white rock having a number equal to the sum of the replaced rocks numbers.<br/>
If a blue rock with the number K is found it is removed and also the previous K rocks are replaced by a white rocks having a number equal to the greatest number of the replaced rocks.<br/>
You must modify the path by following the uplisted rules. Output the modified path.
### Input format ###
N - the length of the path<br/>
Follow N rows from the type: white K or green K or blue K where the first word stands for the colour of the rock and K stands for the number of the rock.
