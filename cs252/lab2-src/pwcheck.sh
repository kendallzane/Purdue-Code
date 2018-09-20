#!/bin/bash
SCORE=0

#length of string
#LENGTH = x
TESTING=$(<$1)
#echo $TESTING

LENGTH=$(echo -n $TESTING | wc -m)

#Simply increment or decrement the SCORE counter the proper amount of points for each criteria

let SCORE=SCORE+LENGTH
#echo $SCORE
#special character
if egrep -q [#$\+%@] $1 ; then
	let SCORE=SCORE+5
fi
#echo $SCORE
#number
if egrep -q [0-9] $1 ; then
	let SCORE=SCORE+5
fi
#echo $SCORE
#alpha
if egrep -q [A-Za-z] $1 ; then
	let SCORE=SCORE+5
fi
#echo $SCORE
#repeating alphanumneric

#:alnum: passes for all alphanumeric characters

if egrep -q [[:alnum:]]\1+ $1 ; then
	let SCORE=SCORE-10
fi
#echo $SCORE
#3 consecutive lowercase
if egrep -q [a-z][a-z][a-z] $1 ; then
	let SCORE=SCORE-3
fi
#echo $SCORE
#3 consecutive uppercase
if egrep -q [A-Z][A-Z][A-Z] $1 ; then
	let SCORE=SCORE-3
fi
#echo $SCORE
#3 consecutive numbers
if egrep -q [0-9][0-9][0-9] $1 ; then
	let SCORE=SCORE-3
fi
#echo $SCORE

a="Password Score: "

#check the length of the password, and print the score if it is the proper length

#echo $LENGTH
if (( $LENGTH < 6 )) || (( $LENGTH > 32 )) ;  then
	echo "Error: Password length invalid."
else
	echo $a$SCORE
fi

