#!/bin/sh
bindir=$(pwd)
cd /home/thibault/Documents/Moteur_de_Jeux/TP1/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /home/thibault/Documents/Moteur_de_Jeux/cmake-build-debug/TP1 
	else
		"/home/thibault/Documents/Moteur_de_Jeux/cmake-build-debug/TP1"  
	fi
else
	"/home/thibault/Documents/Moteur_de_Jeux/cmake-build-debug/TP1"  
fi
