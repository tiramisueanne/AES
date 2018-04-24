SOURCE=$(wildcard *.cc)
TESTING=$(subst encrypt.cc, , $(SOURCE))
BASE=$(subst test.cc, ,$(SOURCE))
EXEC=aes

.PHONY: all clean

# type in "make" to run the program
all: $(BASE)
	chmod +x lint-check.sh
	chmod +x lint-code.sh
	g++ -std=c++14 $(BASE) -o $(EXEC)

# type in "make run" to run the program
run: $(BASE)
	g++ -std=c++14 -g $(BASE) -o $(EXEC)
	./$(EXEC)

.PRECIOUS: test
# type in "make test" to run all unit tests
test: $(TESTING)
	g++ -std=c++14 -g $(TESTING) -o test -lgtest -lgtest_main -pthread
	./test

# type in "clean" to get rid of all executables
clean:
	rm -f $(EXEC) test

# type "make runtest" to run all unit tests and clean up
runtest: test clean

lint-check:
	chmod +x ./lint-check.sh
	bash ./lint-check.sh
