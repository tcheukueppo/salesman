CC      = gcc
OBJECTS = graph.o salesman_seq.o

salesman_seq: $(OBJECTS)
	${CC} -o salesman_seq $(OBJECTS)

salesman_seq.o graph.o: graph.c graph.h
salesman_seq.o: salesman_seq.c

.PHONY: clean
clean:
	rm salesman_seq $(OBJECTS)
