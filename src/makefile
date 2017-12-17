# g++ -Wall -lncurses -std=c++11 define_catch.cpp line.cpp paragraph.cpp printer.cpp reader.cpp updater.cpp main.cpp

#default:
#	g++ -Wall -lncurses -std=c++11 line.cpp paragraph.cpp main.cpp text.cpp -o main

#jtest:
#	g++ -Wall -std=c++11 define_catch.cpp text.cpp line.cpp ../tests/line_tests.cpp -o line_tests

%:
	g++ -Wall -std=c++11 -c $*.cpp -o ../execs/objs/$*.o
