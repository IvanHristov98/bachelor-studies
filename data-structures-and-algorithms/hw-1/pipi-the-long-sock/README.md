### Pipi the long sock ###
Pipi has N socks. Among them there aren't two of the same color. Not only that but the socks have a different length.
Every single day Pipi selects a couple of socks from (N*(N-1))/2 options.
She doesn't want them to repeat, and after all the options had been tested, Pipi decided to throw out the old socks and to buy new ones.
In order not to let any repetition, she ordered the different options in an ascending order of difference between the lengths of the socks.
Whenever there's been an equal difference - the secondary rule of the order had been in an ascending order by the length of the shorter sock in the couple.
In the Kth day Pipi's decided to put on the Kth sock couple option from this ordered list.

Write down a program that finds out which sock couple Pipi must put on the Kth day, if she follows the rules described above.

## Input format ##
On the first row of the standard input are given two integers N and K - respectively number of the socks and  number of the day.
On second row are written down N integers - the lengths of the socks in an ascending order. Numbers are separated by an interval.
