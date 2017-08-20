VPATH = ../objects:../src:../lib:../tests
CC = g++ -Wall -std=c++11

all: line_tests

line_tests: line.o line_tests.o define_catch.o
	$(CC) ../objects/line.o ../objects/line_tests.o ../objects/define_catch.o -o line_tests

../objects/line.o: line.cpp line.hpp
	$(CC) -c ../src/line.cpp -o ../objects/line.o

../objects/line_tests.o: line_tests.cpp line.hpp catch.hpp
	$(CC) -c ../tests/line_tests.cpp -o ../objects/line_tests.o

../objects/define_catch.o: define_catch.cpp catch.hpp
	$(CC) -c ../src/define_catch.cpp -o ../objects/define_catch.o

clean:
	rm line_tests
	rm ../objects/*

	
#FUCK WHAT ABOUT LINE_TESTS That's not in the source folder.
#And this is honestly more confusing than just doing it by hand.
#
#Let's do it by hand for a while and see if an issue comes up. So, do we split into two makefiles or use one makefile in execs?
#Let's just use one in execs.
#
#automatic dependencies could make it easy to write the dependency lines, but not to write the actual compilation lines.
#recipe templates could make this easier. But mostly, I just have to write this I think. I'll see the potential for optimization later.
