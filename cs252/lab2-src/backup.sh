#!/bin/bash

FILE=$1
BACKUPDIR=$2
INTERVAL=$3
MAX=$4

NUMBACKUPS=0

CLEARBACKUPFOLDER=1

#Create the designated backup folder if it doesn't already exist

if [ ! -d $(pwd)/$BACKUPDIR ] ; then
	echo "Created new Directory: $(pwd)/$BACKUPDIR"
	mkdir -p $(pwd)/$BACKUPDIR
fi

#Clear the backup folder of any previously existing backups of this particular file

if [ $CLEARBACKUPFOLDER ] ; then
	echo "CLEARING BACKUP FOLDER"
	rm -rf $(pwd)/$BACKUPDIR/$FILE~*
fi

#Get the date format in Year-Month-Day:Hour:Minute:Seconds

DATE=`date +%Y-%m-%d:%H:%M:%S`

LASTBACKUP=$FILE~$DATE
echo "Monitoring file: $FILE"
echo "Backup Directory: $BACKUPDIR"
echo "Checking file every $INTERVAL seconds"
echo "Max number of backups for file is $MAX"
echo "Initial backup of file: $BACKUPDIR/$LASTBACKUP"

#Create the initial backup

cp $FILE $BACKUPDIR/$LASTBACKUP
let NUMBACKUPS=NUMBACKUPS+1

while [ 1 ]; do
	sleep $INTERVAL
	DATE=`date +%Y-%m-%d:%H:%M:%S`
	DIFF=`diff $FILE $BACKUPDIR/$LASTBACKUP` #Get the difference between the file and the last created backup
	#echo "Diff: $DIFF"
	if [ "$DIFF" != "" ] ; then #if there is a difference


		echo "$DATE - Difference found in file: $FILE"


		LASTBACKUP=$FILE~$DATE #create a new backup file
		echo "Creating new backup: $BACKUPDIR/$LASTBACKUP"

		if (( $NUMBACKUPS >= $MAX )) ; then
			OLDEST=`ls $BACKUPDIR | grep $FILE~ | head -1` #get the oldest backup created for this file, and delete it
			#specifically: Use a REGEX to list all of the files in the backup folder that are based on this file, sort them by the last modified date, and get the head of that list (the oldest file)

			rm $BACKUPDIR/$OLDEST
			echo "Max backups reached - Deleting oldest backup: $BACKUPDIR/$OLDEST"
		else
			let NUMBACKUPS=NUMBACKUPS+1
		fi

		cp $FILE $BACKUPDIR/$LASTBACKUP

		echo ""
		echo $DIFF
		echo ""

		echo $DIFF | /usr/bin/mailx -s "Difference found in file: $FILE" $USER #email the difference to the user
		
		

	else
		echo "$DATE - No difference found in file: $FILE"
	fi

	echo "Number of backups in use: $NUMBACKUPS / $MAX"

done



 
