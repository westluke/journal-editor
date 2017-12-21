CC = g++ -Wall -std=c++11 -lncurses
lines = objs/line.o objs/text.o
catch = objs/define_catch.o

# Wildcard pattern outputs all files matching the pattern. Sometimes a .cpp file will have no
# corresponding headers, so we have to detect that.
# Otherwise, every source has one .cpp file and one.hpp file. If any file breaks this pattern, I add an explicit exception.
# Although I don't really see how something could. WELL it could have extra header files, in that case I just add another rule to combine the dependencies.
objs/%.o: src/%.cpp $(wildcard src/%.hpp)
	$(CC) -c src/$*.cpp -o objs/$*.o

line_tests: $(lines) $(catch) objs/line_tests.o
	$(CC) $^ -o $@

#Do I really want to list every dependency here? Is there a faster way? Could make fake dependency groups for groups of files
main: $(lines) objs/paragraph.o objs/updater.o objs/main.o
	$(CC) $^ -o $@

clean:
	-rm main
	-rm line_tests
	-rm objs/*.o


#%.o:
#	$(CC) -c src/$*.cpp -o objects/$*.o

#line_tests: line.o line_tests.o define_catch.o
#	$(CC) -c 
