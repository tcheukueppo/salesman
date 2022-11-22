CC      = gcc
OBJECTS = graph.o salesman.o salesman_seq.o

salesman: $(OBJECTS)
	${CC} -o salesman $(OBJECTS)

salesman.o salesman_seq.o graph.o: graph.c graph.h
salesman.o salesman_seq.o: salesman_seq.c salesman_seq.h
salesman.o: salesman.c

.PHONY: clean
clean:
	rm salesman $(OBJECTS)
