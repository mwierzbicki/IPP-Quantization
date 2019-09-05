#!/bin/bash
for f in "$2/"*.in
do
	#sztuczka z valgrindem przenoszaca output na file descriptor, liczba deskryptora arbitralna
	valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --log-fd=7 7>temp.log $1 < $f > temp.out 2> temp.err
	cat temp.log
	diff -s ${f%in}out temp.out
	diff -s ${f%in}err temp.err
done

rm -f temp.out
rm -f temp.err
rm -f temp.log
