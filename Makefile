make: test.c allocator.c allocator.h
    gcc -o test test.c allocator.c

clean:
    rm -f test