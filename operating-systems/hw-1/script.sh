#!/bin/bash
cat text.txt | sed -r 's/([A-Z])/\L\1/g' | sed 's/[".,]//g' | sed 's/[ ]/\n/g' | sed '/^$/d' | sort | uniq | xargs -I word sh -c "echo -n 'word:';tre-agrep -B -w -s -E 99 word dic.txt" | egrep '^[a-z]+:[^0]'
