CXX = g++
CC = gcc
CXXFLAGS = -std=c++17 -Iinclude -I/opt/homebrew/include -I/usr/local/include
CFLAGS = -Iinclude -I/opt/homebrew/include -I/usr/local/include
LDFLAGS = -L/opt/homebrew/lib -L/usr/local/lib -lglfw -lglut -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework GLUT

all: app

app: src/main.o
	$(CXX) src/main.o -o app $(LDFLAGS)

src/main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o src/main.o


clean:
	rm -f src/*.o app
