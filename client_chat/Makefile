SRC = main.cpp 
TARGET = chat 
LIB = ChatLib
PREFIX = /usr/local/bin

chat:	$(SRC) lib
	g++ -o $(TARGET) $(SRC) -L. -l$(LIB)

lib:	Client.cpp Client.h	
	g++ -o $(LIB).o Client.cpp -c
	ar rc lib$(LIB).a $(LIB).o

clean:	
	rm -rf *.o *.a 

install:
	install $(TARGET) $(PREFIX)

uninstall:
	rm -rf $(PREFIX)/$(TARGET)


