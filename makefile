.PHONY : clean

CC = gcc -no-pie
EXEC = exec
CFLAGS = -g
#-ansi -Wall -W -pedantic
LDFLAGS = 
LIBS = 

# Finding #
SRC = $(wildcard core/*.c)
OBJS = $(SRC:.c=.o)


# Rules

all : $(EXEC)

$(EXEC) : $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@ 

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $< 

clean:
	@rm -f exec core/coup.o core/dames.o core/gestion_partie.o core/ia.o core/main.o


