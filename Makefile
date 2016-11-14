SRC     = *.c *.h
TARGET  = lightserver
LIB     = -levent -lpthread

$(TARGET):$(SRC)
	gcc -o $(TARGET) $(SRC) $(LIB)
clean:
	rm -f *.o $(TARGET)
