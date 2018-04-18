TARGET=tui

all:
	g++ *.cc -o $(TARGET)

clean:
	rm $(TARGET) 
