SRC = main.cpp 
TARGET = chat 
LIB = ChatLib
PREFIX = /usr/local/bin

chat:	$(SRC) lib
	g++ -o $(TARGET) $(SRC) -L. -l$(LIB)

lib:	Chat.cpp Chat.h User.h Message.h Server.cpp Server.h
	g++ -o $(LIB).o Chat.cpp -c
	g++ -o server.o Server.cpp -c
	ar rc lib$(LIB).a $(LIB).o server.o
clean:	
	rm -rf *.o *.a 

install:
	install $(TARGET) $(PREFIX)

uninstall:
	rm -rf $(PREFIX)/$(TARGET)




