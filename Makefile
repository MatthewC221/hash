all: test unit_pg collision resize time_resize lf_test visual

test: test.c hash.h
	gcc -o test test.c -lm -g

unit_pg: unit_pg.c hash.h
	gcc -o unit_pg unit_pg.c -lm -g
	
collision: collision.c hash.h
	gcc -o collision collision.c -lm -g
	
resize: resize.c hash.h
	gcc -o resize resize.c -lm -g
	
time_resize: time_insert.c hash.h
	gcc -o time_insert time_insert.c -lm -g
	
lf_test: lf_test.c hash.h
	gcc -o lf_test lf_test.c -lm -g    

visual: visual.c hash.h
	gcc -o visual visual.c -lm -g 