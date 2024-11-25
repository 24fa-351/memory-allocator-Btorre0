make: testing.c allocator.c allocator.h
	gcc -o testing testing.c allocator.c -Wno-deprecated-declarations

clean:
	rm -f testing
