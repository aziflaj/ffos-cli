#
# Makefile for ffos-cli
#
# author: Aldo Ziflaj
#

ffos: main.c ffos.c ffos.h
	gcc -std=c99 -o ffos main.c ffos.c -ljansson
	
clean:
	rm -f *.o a.out core ffos
