TARGET=encryptotron

all:
	g++ *.cc -o $(TARGET)

clean:
	rm $(TARGET) 
