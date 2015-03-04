CFLAGS=-std=c++0x -I./inc
LFLAGS=-L./lib -lglfw3 -lopengl32 -lgdi32

HW2.exe: main.o hw2.o glwrapper.o
	g++ -o HW2.exe main.o hw2.o glwrapper.o $(LFLAGS)
main.o: main.cpp hw2.h
	g++ $(CFLAGS) -c main.cpp
hw2.o: hw2.h
	g++ $(CFLAGS) -c hw2.cpp
glwrapper.o: glwrapper.h
	g++ $(CFLAGS) -c glwrapper.cpp
clean:
	del *.o *.exe