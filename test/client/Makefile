SRC     = *.c *.h
TARGET  = tcpclient
LIB     = -lpthread

$(TARGET):$(SRC)
	gcc -o $(TARGET) $(SRC) $(LIB)
clean:
	rm -f *.o $(TARGET)
