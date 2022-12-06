CC      = gcc
OBJECTS = graph.o salesman.o salesman_seq.o salesman_thr.o mcost.o

salesman: $(OBJECTS)
	${CC} -o salesman $(OBJECTS)

graph.o: graph.c graph.h
salesman.o salesman_seq.o salesman_thr.o: graph.c graph.h common.h mcost.h
salesman.o salesman_seq.o: salesman_seq.c salesman_seq.h
salesman.o salesman_thr.o: salesman_thr.c salesman_thr.h
salesman.o: salesman.c arg.h

.PHONY: clean
clean:
	rm salesman $(OBJECTS)
