SRC_FILES := ./src/*c
SRC_FILES += main.c
INC := ./inc

all:
	gcc $(SRC_FILES) -I$(INC) -o server -lpthread -ljansson

clean:
	rm -rf *.o
	rm -f server
	rm -f tester
