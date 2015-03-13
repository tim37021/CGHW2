CXX = g++

CXXFLAGS = -std=c++0x -g -I./inc
LIBS = -L./lib -lglfw3 -lopengl32 -lgdi32

OUT_EXE = HW2.exe

all: $(OUT_EXE)

$(OUT_EXE): main.o hw2.o glwrapper.o math.o mesh.o srenderer.o
	$(CXX) $^ $(LIBS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean
clean:
	del *.o $(OUT_EXE)
