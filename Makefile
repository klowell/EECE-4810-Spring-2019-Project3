CC = gcc
CFLAGS = -std=c99 -Wall
OBJECTS = Project3.o sched_sim.o
EXECS = Project3

all: $(EXECS)

$(EXECS) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXECS) $(OBJECTS)

clean:
	-rm $(OBJECTS) $(EXECS)
