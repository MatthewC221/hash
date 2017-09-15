all: glib_collision_speed time_test collision_speed time_test_glib time_test_str time_test_exp time_test_khash glib_collision_speed_lookup collision_speed_lookup

glib_collision_speed: glib_collision_speed.c 
	gcc -o glib_collision_speed glib_collision_speed.c -lm -g `pkg-config --cflags --libs glib-2.0`

glib_collision_speed_lookup: glib_collision_speed_lookup.c 
	gcc -o glib_collision_speed_lookup glib_collision_speed_lookup.c -lm -g `pkg-config --cflags --libs glib-2.0`

time_test_khash:
	gcc -o time_test_khash time_test_khash.c

time_test_glib: time_test_glib.c
	gcc -o time_test_glib time_test_glib.c -lm -g `pkg-config --cflags --libs glib-2.0`

time_test: time_test.c ../hash.h ../hash.c
	gcc -o time_test time_test.c ../hash.c -lm -g

time_test_exp: time_test_exp.c ../exp_hash.h ../exp_hash.c
	gcc -o time_test_exp time_test_exp.c ../exp_hash.c -lm -g

collision_speed: collision_speed.c ../hash.h ../hash.c
	gcc -o collision_speed collision_speed.c ../hash.c -lm -g

collision_speed_lookup: collision_speed_lookup.c ../hash.h ../hash.c
	gcc -o collision_speed_lookup collision_speed_lookup.c ../hash.c -lm -g

time_test_str: time_test_str.c ../hash.h ../hash.c
	gcc -o time_test_str time_test_str.c ../hash.c -lm -g

clean:
	rm -rf hash.o