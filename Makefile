CC = gcc
TARGETS = bin/socket_writer \
	      bin/socket_reader \
	      bin/shm_writer \
		  bin/shm_reader \
		  bin/fifo_writer \
		  bin/fifo_reader \
		  bin/mq_writer \
		  bin/mq_reader \
		  bin/unnamed_pipes

SRC = src/socket_writer.c \
	  src/socket_reader.c \
	  src/shm_writer.c \
	  src/shm_reader.c \
	  src/fifo_writer.c \
	  src/fifo_reader.c \
	  src/mq_writer.c \
	  src/mq_reader.c \
	  src/unnamed_pipes.c

all: directories $(TARGETS)		  
.PHONY: directories
directories:
	mkdir -p results
	mkdir -p bin

bin/% : src/%.c
	$(CC) -o $@ $< -lrt -pthread

clean:
	rm -f bin/* results/*