SRC     = *.c *.h
TARGET  = lightserver
LIB     = -levent

$(TARGET):$(SRC)
	gcc -o $(TARGET) $(SRC) $(LIB)
clean:
	rm -f *.o $(TARGET)
