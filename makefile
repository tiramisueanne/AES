SOURCE=$(wildcard *.cc)
TESTING=$(subst encrypt.cc, , $(SOURCE))
BASE=$(subst test.cc, ,$(SOURCE))
EXEC=aes

.PHONY: all clean

# type in "make" to run the program
all: $(BASE)
	g++ -std=c++14 $(BASE) -o $(EXEC)

# type in "make test" to run all unit tests
test: $(TESTING)
	g++ -std=c++14 $(TESTING) -o test -lgtest -lgtest_main -pthread
	./test

# type in "clean" to get rid of all executables
clean:
	rm -f $(EXEC) test

# type "make runtest" to run all unit tests and clean up
runtest: test clean
