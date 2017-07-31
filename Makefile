all: test unit_pg collision resize time_resize lf_test open_addr collision_speed

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

open_addr: open_addr.c hash.h
	gcc -o open_addr open_addr.c -lm -g 

collision_speed: collision_speed.c hash.h
	gcc -o collision_speed collision_speed.c -lm -g