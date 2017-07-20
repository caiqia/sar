.PHONY : all clean check
.ONESHELL:

CHECK_PATCH=../../../../biblio/eudyptula/torvalds/scripts/checkpatch.pl

all : libfunc.so libread.so cron_func

nothing.o:nothing.c
	gcc -Wall -c -fPIC $<

read.o:read.c
	gcc -Wall -c -fPIC $<

%.o : %.c
	gcc -Wall -c $<

cron_func : cron_func.o nothing.o read.o
	gcc -Wall $^ -o $@

#gcc -shared  -fPIC -o libfunc.so.1.0[*.c] -W1,soname,libfunc.so.1

libfunc.so : nothing.o
	gcc -Wall -shared $^ -o $@

libread.so : read.o
	gcc -Wall -shared $^ -o $@ -ldl
	LD_PRELOAD=./libread.so ./cron_func


check :
	for f in *.c *.h ; do
		$(CHECK_PATCH) -f $$f
	done

clean :
	rm -rf *.o cron_func
