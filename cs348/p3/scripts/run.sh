echo exit | sqlplus bowlesk@csora/ktNAfbOn @drop.sql >/dev/null
echo exit | sqlplus bowlesk@csora/ktNAfbOn @create.sql >/dev/null
echo exit | sqlplus bowlesk@csora/ktNAfbOn @init.sql >/dev/null

javac -cp .:ojdbc8.jar Project3.java
java -cp .:ojdbc8.jar Project3 input.txt output.txt
