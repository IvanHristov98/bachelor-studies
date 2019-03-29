## Rename ##
There is course of student and lector wants to know the name of each of them. He wants to optimize the process and so for that reason each student has a new shortened name - the one the lector would be using to turn to the student.
Having an ordered list of students the name of a given student is the prefix which isn't a prefix of the name of another student, already added to the list.
Your task is to generate the list with the names of the students.

If the whole name of a student is already met then his/her new name is old_name + number(delimited by a space), where the number represents the count of the times old_name has already been added.
### Input format ###
On the first row of the standard input you are given a number N representing the number of the students.<br/>
It is followed by N rows containing the original names of the students.
