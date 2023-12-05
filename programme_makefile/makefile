parcEolien.o : parcEolien.h parcEolien.c
	gcc -c parcEolien.c

main.o : tas.h main.c
	gcc -c test_parcEolien.c

main : tas.o main.o
	gcc parcEolien.o main.o -o main
