TARGET=	dice
OBJS=	dice.o stack.o

CC=	cc
RM=	rm -f
SHELL=	/bin/sh

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)
