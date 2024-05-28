CXX=g++
CXXFLAGS=-Wall
LDFLAGS=-lSDL2 -lGLEW -lGLU -lGL

OUT=gl.out

legacy_opengl_triangle: legacy_opengl_triangle.cpp
	 $(CXX) legacy_opengl_triangle.cpp -o $(OUT) $(LDFLAGS)

modern_opengl_triangle: modern_opengl_triangle.cpp
	 $(CXX) modern_opengl_triangle.cpp -o $(OUT) $(LDFLAGS)

modern_opengl_square: modern_opengl_square.cpp
	 $(CXX) modern_opengl_square.cpp -o $(OUT) $(LDFLAGS)
