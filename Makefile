build: testparse.cpp lispparser.cpp lispparser.h
		g++ -std=c++11 -Wall main.cpp lispparser.cpp -o testparse

clean:
		rm -f testparse

rebuild: clean build

test: testparse
		./run_tests.sh

