all: test unit_pg collision resize open_addr collision_speed time_test compute_mod time_test_glib
# lf_test left out
# time_test_str time_test_glib

test: test.c hash.h
	gcc -o test test.c -lm -g `pkg-config --cflags --libs glib-2.0`

time_test_glib: time_test_glib.c hash.h
	gcc -o time_test_glib time_test_glib.c -lm -g `pkg-config --cflags --libs glib-2.0`

# unit_pg: unit_pg.c hash.h hash.c
# 	gcc -o unit_pg unit_pg.c hash.c -lm -g

#collision: collision.c hash.h hash.c
#	gcc -o collision collision.c hash.c -lm -g
	
#resize: resize.c hash.h hash.c
#	gcc -o resize resize.c hash.c -lm -g
	
# time_resize: time_insert.c hash.h
#	gcc -o time_insert -pg time_insert.c -lm -g
	
#lf_test: lf_test.c hash.h hash.c
#	gcc -o lf_test lf_test.c hash.c -lm -g  

#open_addr: open_addr.c hash.h hash.c
#	gcc -o open_addr open_addr.c hash.c -lm -g

time_test_str: time_test_str.c hash.h hash.c
	gcc -o time_test_str time_test_str.c hash.c -lm -g

collision_speed: collision_speed.c hash.h hash.c
	gcc -o collision_speed collision_speed.c hash.c -lm -g

time_test: time_test.c hash.h hash.c
	gcc -o time_test time_test.c hash.c -lm -g

#compute_mod: compute_mod.c
#	gcc -o compute_mod compute_mod.c  

clean:
	rm -rf hash.o