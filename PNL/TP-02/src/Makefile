.PHONY : all clean check
.ONESHELL:

CHECK_PATCH=./checkpatch.pl --no-tree

all : testHistory testCommit

%.o : %.c
	gcc -Wall -c $<

testHistory : testHistory.c commit.o version.o history.o
	gcc -Wall $^ -o $@

testCommit : testCommit.c commit.o version.o
	gcc -Wall $^ -o $@

check : 
	for f in *.c *.h ; do
		$(CHECK_PATCH) -f $$f
	done

clean :
	rm -rf *.o testHistory
