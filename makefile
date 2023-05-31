SRC = main.cpp 
TARGET = chat 
LIB = ChatLib
PREFIX = /usr/local/bin

chat:	$(SRC) lib
	g++ -o $(TARGET) $(SRC) -L. -l$(LIB)

lib:	Chat.cpp Chat.h User.h Message.h	
	g++ -o $(LIB).o Chat.cpp -c
	ar rc lib$(LIB).a $(LIB).o

clean:	
	rm -rf *.o *.a 

install:
	install $(TARGET) $(PREFIX)

uninstall:
	rm -rf $(PREFIX)/$(TARGET)





