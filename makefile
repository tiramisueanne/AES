SOURCE=$(wildcard *.cc)
TESTING=$(subst encrypt.cc, , $(SOURCE))
BASE=$(subst test.cc, ,$(SOURCE))
TARGET=tui

all:
	g++ -std=c++14 $(BASE) -o $(TARGET)

testIt: test
	./test	

test:
	g++ -std=c++14 $(TESTING) -o test -lgtest -lgtest_main -pthread

clean:
	rm $(TARGET) test
