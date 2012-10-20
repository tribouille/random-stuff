all: test-yalj

test-yalj:
	gcc -L /usr/local/lib/ -lyajl test.c
