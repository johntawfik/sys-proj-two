CC=gcc

CFLAGS=-Wall -g

TARGET=spchk

TEST_DICT=/usr/share/dict/words
TEST_DIR=test_dir
TEST_FILE=my_file.txt

all: $(TARGET)

$(TARGET): spchk.o
	$(CC) $(CFLAGS) spchk.o -o $(TARGET)

spchk.o: spchk.c
	$(CC) $(CFLAGS) -c spchk.c

test: $(TARGET)
	./$(TARGET) $(TEST_DICT) $(TEST_DIR) $(TEST_FILE)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean test
