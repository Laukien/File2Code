CC := gcc
CFLAGS += -Wall -O3
#CFLAGS += -ggdb3
NAME := file2code

all: init build

init:
	mkdir -p bin

build:
	$(CC) $(CFLAGS) -o bin/$(NAME) src/main.c

test: build
	bin/$(NAME) README
	$(CC) $(CFLAGS) -o bin/example src/example.c README.c
	bin/example

nice:

install:
	strip bin/$(NAME)
	upx bin/$(NAME)
	cp bin/$(NAME) /usr/local/bin

deinstall:
	rm -f /usr/local/bin/$(NAME)

