#!/bin/bash

cmdfile=/tmp/doafterchanges
touch $cmdfile

if [ $# -lt 2 ]; then
    echo "Usage: $0 cmd watched_files"
    exit 1
fi

which inotifywait 2>&1 >/dev/null

if [ "$?" == "1" ]; then
    echo "You need inotifywait installed"
    exit 1
fi

cmd=$1
shift
arg_files=$*
files=

for list_files in $arg_files;
do
	files+=$(echo "$(find . -iname "${list_files}") ")
done


echo $files

function exec_cmd () {
    echo "[`date`] >>>> Run: $cmd"
    eval $cmd
    echo "[`date`] >>>> Exit code: $?"
}

# do it first
exec_cmd

while true; do
    # inotifywait -t 60 -q -r $files
    inotifywait -q -r --event MOVE_SELF $files $cmdfile
    exec_cmd
done

