CC = g++
CFLAGS = -Wall -lncurses
SOURCES = GameField.cpp ai.cpp clui.cpp
OBJECTS = GameField.o ai.o clui.o
EXECUTABLE = lpgomoku

.PHONY: all clean

all: $(EXECUTABLE)

clean:
	@rm -rf $(EXECUTABLE) $(OBJECTS)

$(OBJECTS):
	@$(CC) -c $(CFLAGS) $(SOURCES)

$(EXECUTABLE): $(OBJECTS)
	@$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJECTS)
	