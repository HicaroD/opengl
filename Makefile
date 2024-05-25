CXX=g++
CXXFLAGS=-Wall
LDFLAGS=-lSDL2 -lGLEW -lGLU -lGL

OUT=gl.out

hello: hello.cpp
	 $(CXX) hello.cpp -o $(OUT) $(LDFLAGS)
