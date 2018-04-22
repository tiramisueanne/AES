SOURCE=$(wildcard *.cc)
TESTING=$(subst encrypt.cc, , $(SOURCE))
BASE=$(subst test.cc, ,$(SOURCE))
EXEC=tui

# type in "make run" to run the program
run: $(EXEC)
	g++ -std=c++14 $(BASE) -o $(EXEC)
	./$(EXEC)

# type in "test" to run all unit tests
test:
	g++ -std=c++14 $(TESTING) -o test -lgtest -lgtest_main -pthread
	./test	

#type in "clean" to get rid of all executables
clean:
	rm $(TARGET) test
