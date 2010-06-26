all: main.cpp lispparser.cpp
		g++ -g main.cpp lispparser.cpp -o lispparser