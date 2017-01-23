SRC     = *.c *.h
TARGET  = lightserver
LIB     = -levent -lpthread -lcrypto -lmysqlclient

$(TARGET):$(SRC)
	gcc -o $(TARGET) $(SRC) $(LIB)
clean:
	rm -f *.o $(TARGET)
logclean:
	rm -f log/dev/*.txt
	rm -f log/monitor/*.txt
