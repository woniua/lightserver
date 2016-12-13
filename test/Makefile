SRC     = *.c
TARGET  = tcpclient
LIB     = -lpthread
OPT     = -std=gnu99

$(TARGET):$(SRC)
	gcc -o $(TARGET) $(OPT) $(SRC) $(LIB)
clean:
	rm -f *.o $(TARGET)
