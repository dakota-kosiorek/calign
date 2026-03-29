CC = gcc
CFLAGS = -g -Wall
TARGET = calign

default: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -f $(TARGET)
