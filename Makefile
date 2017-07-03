all: test unit_pg collision resize

test: test.c hash.h
	gcc -o test test.c -lm -g

unit_pg: unit_pg.c hash.h
	gcc -o unit_pg unit_pg.c -lm -g
	
collision: collision.c hash.h
	gcc -o collision collision.c -lm -g
	
resize: resize.c hash.h
	gcc -o resize resize.c -lm -g
