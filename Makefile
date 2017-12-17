CC = g++ -Wall -std=c++11 -lncurses

# Wildcard pattern outputs all files matching the pattern. Sometimes a .cpp file will have no
# corresponding headers, so we have to detect that.
# Otherwise, every source has one .cpp file and one.hpp file. If any file breaks this pattern, I add an explicit exception.
# Although I don't really see how something could. WELL it could have extra header files, in that case I just add another rule to combine the dependencies.
objs/%.o: src/%.cpp $(wildcard src/%.hpp)
	$(CC) -c src/$*.cpp -o objs/$*.o

line_tests: objs/line.o objs/line_tests.o objs/define_catch.o objs/text.o
	$(CC) $^ -o $@

#%.o:
#	$(CC) -c src/$*.cpp -o objects/$*.o

#line_tests: line.o line_tests.o define_catch.o
#	$(CC) -c 
