all: main.o QuantDict.o FindUnion.o
	gcc -o quantization main.o QuantDict.o FindUnion.o

main.o : main.c defines.h QuantDict.h
	gcc -Wall -Wextra -std=c11 -O2 -c main.c

QuantDict.o : QuantDict.c QuantDict.h FindUnion.h
	gcc -Wall -Wextra -std=c11 -O2 -c QuantDict.c

FindUnion.o : FindUnion.c FindUnion.h
	gcc -Wall -Wextra -std=c11 -O2 -c FindUnion.c

clean :
	rm -f main.o
	rm -f QuantDict.o
	rm -f FindUnion.o
	rm -f quantization

