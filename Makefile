build: test_parse.cpp lispparser.cpp lispparser.h
		g++ -std=c++11 -Wall test_parse.cpp lispparser.cpp -o test_parse

clean:
		rm -f test_parse

rebuild: clean build

test: rebuild
		./run_tests.sh

