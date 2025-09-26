GENERATES = README prog liboutput_static.a liboutput.so prog-a prog-so
CFLAGS = -Wall
TRASH = *.o *~ o.* *.out

all:	README prog liboutput_static.a liboutput.so prog-a prog-so

fun.o:	outlib.h

prog:	const.o fun.o prog.o

prog-a:	prog.o liboutput_static.a
	cc -o $@ $< -L. -loutput_static

prog-so:	prog.o liboutput.so
	cc -o $@ $< -L. -loutput -Wl,-rpath,.

README:	prog
	./$< 2> $@

liboutput_static.a:	const.o fun.o
	ar -rcs $@ $^

liboutput.so:	fun.o const.o
	cc -shared $^ -o $@

test:	prog prog-a prog-so
	@if [ -n "$(ARG3)" ]; then \
		bash -c "exec -a prog ./prog $(ARG3)" > test-prog.out 2>&1; \
		bash -c "exec -a prog ./prog-a $(ARG3)" > test-prog-a.out 2>&1; \
		bash -c 'exec -a prog ./prog-so $(ARG3)' > test-prog-so.out 2>&1; \
	elif [ -n "$(ARG1)" ]; then \
		bash -c "exec -a prog ./prog $(ARG1)" > test-prog.out 2>&1; \
		bash -c "exec -a prog ./prog-a $(ARG1)" > test-prog-a.out 2>&1; \
		bash -c "exec -a prog ./prog-so $(ARG1)" > test-prog-so.out 2>&1; \
	else \
		bash -c "exec -a prog ./prog" > test-prog.out 2>&1; \
		bash -c "exec -a prog ./prog-a" > test-prog-a.out 2>&1; \
		bash -c "exec -a prog ./prog-so" > test-prog-so.out 2>&1; \
	fi
	cmp test-prog.out test-prog-a.out
	cmp test-prog.out test-prog-so.out

clean:
	rm -f $(TRASH)

distclean:	clean
	rm -rf $(GENERATES)
