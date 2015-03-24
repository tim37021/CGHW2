CXX = g++
CC = gcc

CXXFLAGS = -std=c++0x -DGLEW_STATIC -O2 -I./inc
CCFLAGS = -std=c99 -DGLEW_STATIC -O2 -I./inc
LIBS = -L./lib -lglfw3 -lopengl32 -lgdi32

OUT_EXE = HW2.exe

all: $(OUT_EXE)

$(OUT_EXE): main.o mesh.o srenderer.o framebuffer.o glew.o shader.o vertex.o texture.o
	$(CXX) $^ $(LIBS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

.PHONY: clean
clean:
	del *.o $(OUT_EXE)
