PROGRAMS = naive bitcount mergecount mergecount2 treecount
CC = gcc
CFLAGS = -O3 -march=core2 -msse4.1

INPUT = crossing.txt

all: $(PROGRAMS)
	for i in $(PROGRAMS); do time ./$$i $(INPUT); done

$(foreach p, $(PROGRAMS), $(p)-10): %-10: %
	time (for i in 1 2 3 4 5 6 7 8 9 10; do ./$* $(INPUT); done)

clean:
	rm $(PROGRAMS)
