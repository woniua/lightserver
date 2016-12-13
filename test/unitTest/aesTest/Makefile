SRC     = *.c ../../../aes_options.c
TARGET  = aesTest
LIB     = -lcrypto

$(TARGET):$(SRC)
	gcc -o $(TARGET) $(SRC) $(LIB)
clean:
	rm -f *.o $(TARGET)
