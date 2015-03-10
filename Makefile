CXX = g++

CXXFLAGS = -std=c++0x -g `pkg-config --cflags sdl2 gl`
LIBS = `pkg-config --libs sdl2 gl`

OUT_EXE = HW2

all: $(OUT_EXE)

$(OUT_EXE): main.o hw2.o glwrapper.o
	$(CXX) $^ $(LIBS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean
clean:
	rm *.o $(OUT_EXE)
