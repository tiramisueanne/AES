TARGET=tui

all:
	g++ -std=c++14 *.cc -o $(TARGET)

clean:
	rm $(TARGET) 
