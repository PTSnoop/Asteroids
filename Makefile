all: main animage
	g++ main.o animage.o -lIrrlicht -o a.out

main: main.cpp
	g++ -c main.cpp -I/usr/include/irrlicht

animage: animage.cpp
	g++ -c animage.cpp -I/usr/include/irrlicht

run: all
	./a.out
