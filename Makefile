output: CAOS.o
    gcc CAOS.o -o output

CAOS.c: CAOS.c
    gcc -c CAOS.c

clean:
    rm *.o output