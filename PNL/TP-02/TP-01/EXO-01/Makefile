.PHONY : all clean check
.ONESHELL:

CHECK_PATCH=./checkpatch.pl --no-tree

all : testVersion testOffset

%.o : %.c
	gcc -Wall -c $<

testVersion : testVersion.c version.o
	gcc -Wall $^ -o $@

testOffset : testOffset.c 
	gcc -Wall $^ -o $@

check : 
	for f in *.c *.h ; do
		$(CHECK_PATCH) -f $$f
	done

clean :
	rm -rf *.o testVersion testOffset
