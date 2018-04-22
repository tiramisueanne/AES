SOURCE=$(wildcard *.cc)
TESTING=$(subst encrypt.cc, , $(SOURCE))
BASE=$(subst test.cc, ,$(SOURCE))
TARGET=tui

run: $(TARGET)
	g++ -std=c++14 $(BASE) -o $(TARGET)
	./$(TARGET)


test:
	g++ -std=c++14 $(TESTING) -o test -lgtest -lgtest_main -pthread
	./test	

clean:
	rm $(TARGET) test
