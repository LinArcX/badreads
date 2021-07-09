CC = gcc
LIBS = -lm -ldl
CFLAGS =  -Wall

NAME = badreads
DEBUG = output/debug
RELEASE = output/release

.PHONY: all debug release clean

all: release

debug: ../../src/core/extractor.c ../../src/core/util.c ../../src/core/main.c
	mkdir -p output/$@
	$(CC) $(CFLAGS) $(LIBS) -o $(DEBUG)/$(NAME) $^ -g

release: ../../src/core/extractor.c ../../src/core/util.c ../../src/core/main.c
	mkdir -p output/$@
	$(CC) $(CFLAGS) $(LIBS) -o $(RELEASE)/$(NAME) $^

clean:
	rm -f $(DEBUG)/*
	rm -f $(RELEASE)/*
