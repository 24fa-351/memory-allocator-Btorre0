make: test.c allocator.c allocator.h
	gcc -o test test.c allocator.c allocator.h

clean:
	rm -f test