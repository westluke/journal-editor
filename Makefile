CC = g++ -Wall -std=c++11 -lncurses
lines = objs/line.o objs/text.o
catch = objs/define_catch.o

# Wildcard pattern outputs all files matching the pattern. Sometimes a .cpp file will have no
# corresponding headers, so we have to detect that.
# Otherwise, every source has one .cpp file and one.hpp file. If any file breaks this pattern, I add an explicit exception.
# Although I don't really see how something could. WELL it could have extra header files, in that case I just add another rule to combine the dependencies.
# Interesting. This isn't working for headers.
#
# That's because wildcard accepts a file pattern. % isn't part of that pattern, it's not expanded there.
# I might have been able to use secondary expansion though.

objs/%.o: src/%.cpp src/%.hpp
	$(CC) -c src/$*.cpp -o objs/$*.o

objs/%.o: src/%.cpp
	$(CC) -c src/$*.cpp -o objs/$*.o

line_tests: $(lines) $(catch) objs/line_tests.o
	$(CC) $^ -o $@

main: $(lines) objs/paragraph.o objs/document.o objs/main.o objs/display.o objs/debug_stream.o
	$(CC) $^ -o $@

clean:
	-rm main
	-rm line_tests
	-rm objs/*.o


#%.o:
#	$(CC) -c src/$*.cpp -o objects/$*.o

#line_tests: line.o line_tests.o define_catch.o
#	$(CC) -c 
