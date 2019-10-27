.PHONY : clean

CC = gcc -no-pie
EXEC = exec
CFLAGS = -g
#-ansi -Wall -W -pedantic
LDFLAGS = 
LIBS = 

# Finding #
SRC = $(wildcard *.c)
OBJS = $(SRC:.c=.o)


# Rules

all : $(EXEC)

$(EXEC) : $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@ 

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $< 

clean:
	@rm -f exec coup.o dames.o gestion_partie.o ia.o main.o


